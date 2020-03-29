/*
 * Executable Parser Implementation
 *
 * 2018, Operating Systems
 */

#include <stdio.h>
#include <elf.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#include "exec_parser.h"

#define BUFSIZE 1024

static void fix_auxv(uintptr_t base, char *envp[])
{
	Elf32_auxv_t *auxv;
	Elf32_Ehdr *ehdr;
	Elf32_Phdr *phdr;

	ehdr = (Elf32_Ehdr *)base;
	phdr = (Elf32_Phdr *)((uintptr_t)ehdr + ehdr->e_phoff);

	while (*envp)
		envp++;

	auxv = (Elf32_auxv_t *)(++envp);

	while (*envp)
		envp++;

	while (auxv->a_type != AT_NULL) {
		switch (auxv->a_type) {
		case AT_PHDR:
			auxv->a_un.a_val = (uint32_t)(unsigned long)phdr;
			break;
		case AT_BASE:
			auxv->a_un.a_val = 0;
			break;
		case AT_ENTRY:
			auxv->a_un.a_val = ehdr->e_entry;
			break;
		case AT_EXECFN:
			auxv->a_un.a_val = 0;
			break;
		}
		auxv++;
	}

}


void so_start_exec(so_exec_t *exec, char *argv[])
{
	int *pargc;

	fix_auxv(exec->base_addr, __environ);
	/* fix argv to use the one from the main prog */
	argv--;

	pargc = (int *)argv - 1;

	pargc[1] = pargc[0] - 1;

	asm volatile(
		"mov %0, %%eax\n"
		"mov %1, %%ebx\n"
		"mov %%ebx, %%esp\n"
		"xor %%ebx, %%ebx\n"
		"xor %%ecx, %%ecx\n"
		"xor %%edx, %%edx\n"
		"xor %%ebp, %%ebp\n"
		"xor %%esi, %%esi\n"
		"xor %%edi, %%edi\n"
		"jmp *%%eax\n"
		::"m"(exec->entry), "m"(argv) :);
}

so_exec_t *so_parse_exec(char *path)
{
	so_exec_t *exec = NULL;
	so_seg_t *seg;
	char hdr[BUFSIZE];
	int ret;
	Elf32_Ehdr *ehdr;
	Elf32_Phdr *phdr;
	int i;
	int j;
	int num_load_phdr;
	int pagesz;
	int fd;
	size_t diff;

	pagesz = getpagesize();

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		perror("open");
		goto out;
	}

	ret = read(fd, hdr, BUFSIZE);
	if (ret < 0) {
		perror("read");
		goto out_close;
	}

	if (ret < (sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr))) {
		fprintf(stderr, "file too small\n");
		goto out_close;
	}

	ehdr = (Elf32_Ehdr *)hdr;
	phdr = (Elf32_Phdr *)((intptr_t)ehdr + ehdr->e_phoff);

	/* allow only 32-bit ELF executables (no PIE) for i386 */
	if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
	    ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
	    ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
	    ehdr->e_ident[EI_MAG3] != ELFMAG3) {
		fprintf(stderr, "not an ELF file: invalid magic\n");
		goto out_close;
	}

	if (ehdr->e_ident[EI_CLASS] != ELFCLASS32) {
		fprintf(stderr, "not a 32-bit ELF file\n");
		goto out_close;
	}

	if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {
		fprintf(stderr, "not a LSB ELF file\n");
		goto out_close;
	}

	if (ehdr->e_ident[EI_VERSION] != EV_CURRENT) {
		fprintf(stderr, "invalid EI_VERSION\n");
		goto out_close;
	}

	if (ehdr->e_ident[EI_OSABI] != ELFOSABI_GNU &&
	    ehdr->e_ident[EI_OSABI] != ELFOSABI_SYSV) {
		fprintf(stderr, "invalid ABI\n");
		goto out_close;
	}

	if (ehdr->e_type != ET_EXEC) {
		fprintf(stderr, "invalid executable type\n");
		goto out_close;
	}

	if (ehdr->e_machine != EM_386) {
		fprintf(stderr, "invalid machine\n");
		goto out_close;
	}

	if (ehdr->e_version != EV_CURRENT) {
		fprintf(stderr, "invalid version\n");
		goto out_close;
	}

	if (ret < (sizeof(Elf32_Ehdr) + ehdr->e_phnum * ehdr->e_phentsize)) {
		fprintf(stderr, "too many program headers\n");
		goto out_close;
	}

	exec = malloc(sizeof(*exec));
	if (!exec) {
		fprintf(stderr, "out of memory\n");
		goto out_close;
	}

	num_load_phdr = 0;
	for (i = 0; i < ehdr->e_phnum; i++) {
		if (phdr[i].p_type == PT_LOAD)
			num_load_phdr++;
	}

	exec->base_addr = 0xffffffff;
	exec->entry = ehdr->e_entry;
	exec->segments_no = num_load_phdr;
	exec->segments = (so_seg_t *)malloc(num_load_phdr * sizeof(so_seg_t));

	/* convert ELF phdrs to so_segments */
	j = 0;
	for (i = 0; i < ehdr->e_phnum; i++) {
		if (phdr[i].p_type == PT_LOAD) {
			seg = &exec->segments[j];

			seg->vaddr = ALIGN_DOWN(phdr[i].p_vaddr, pagesz);
			diff = phdr[i].p_vaddr - seg->vaddr;
			seg->offset = phdr[i].p_offset - diff;
			seg->file_size = phdr[i].p_filesz + diff;
			seg->mem_size = phdr[i].p_memsz + diff;
			seg->perm = 0;

			if (phdr[i].p_flags & PF_X)
				seg->perm |= PERM_X;
			if (phdr[i].p_flags & PF_R)
				seg->perm |= PERM_R;
			if (phdr[i].p_flags & PF_W)
				seg->perm |= PERM_W;

			if (seg->vaddr < exec->base_addr)
				exec->base_addr = seg->vaddr;

			j++;
		}
	}

out_close:
	close(fd);
out:
	return exec;
}
