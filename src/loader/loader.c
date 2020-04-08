/*
 * Loader Implementation
 *
 * 2018, Operating Systems
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "exec_parser.h"
#include <stdbool.h>

#define SIGSEGV_ERROR 139

typedef struct page
{
	void *pageAddress;
	struct page *nextPage;
} Page;

typedef struct list
{
	Page *cachedPages;
	int pageSize;
} Loader;

static so_exec_t *exec;
static int exec_decriptor;
static Loader *loader;

void init(Loader **loader)
{
	*loader = malloc(sizeof(Loader));
	(*loader)->cachedPages = NULL;
	(*loader)->pageSize = getpagesize();
}

void addPage(void *pageAddress, Loader *loader)
{
	Page *newPage = malloc(sizeof(Page));
	newPage->pageAddress = pageAddress;
	newPage->nextPage = loader->cachedPages;
	loader->cachedPages = newPage;
}

bool find(void *pageAddress, Loader *loader)
{
	Page *cachedPage = loader->cachedPages;
	while (cachedPage)
	{
		if (((char *)pageAddress - (char *)cachedPage->pageAddress) < loader->pageSize && (pageAddress - cachedPage->pageAddress) > 0)
			return true;
		cachedPage = cachedPage->nextPage;
	}
	return false;
}

ssize_t xread(int fd, void *buf, size_t count)
{
	size_t bytes_read = 0;

	while (bytes_read < count)
	{
		ssize_t bytes_read_now =
			read(fd, buf + bytes_read,
				 count - bytes_read);

		if (bytes_read_now == 0) /* EOF */
			return bytes_read;

		if (bytes_read_now < 0) /* I/O error */
			return -1;

		bytes_read += bytes_read_now;
	}

	return bytes_read;
}

void copy_into(so_seg_t *segment, int offset, void *pageAddress)
{
	ssize_t pageSize = getpagesize();
	char *buffer = calloc(pageSize, sizeof(char));
	printf("Start to read from here: %d but file has %d, segment offset is %d\n ", segment->offset + offset, segment->file_size, segment->offset);
	int bytesRead;
	if (offset < segment->file_size)
	{
		lseek(exec_decriptor, segment->offset + offset, SEEK_SET);
		bytesRead = xread(exec_decriptor, buffer, pageSize);
		memcpy(pageAddress, buffer, pageSize);
	}
	if(segment->mem_size - offset > pageSize)
		memcpy(pageAddress, buffer, pageSize);
	else memcpy(pageAddress,buffer,segment->mem_size - offset);
}

so_seg_t *find_segment_of(void *addr)
{
	int diff;
	for (int i = 0; i < exec->segments_no; i++)
	{
		diff = (char *)addr - (char *)exec->segments[i].vaddr;
		if (diff < exec->segments[i].mem_size && diff >= 0)
			return &(exec->segments[i]);
	}
	return NULL;
}

static void signal_handler(int sig, siginfo_t *si, void *unused)
{
	size_t pagesize = getpagesize();
	so_seg_t *segment = find_segment_of(si->si_addr);
	size_t segment_offset = (char *)si->si_addr - (char *)segment->vaddr;
	size_t page_offset = segment_offset % pagesize;
	segment_offset -= page_offset;

	if (!segment)
		exit(SIGSEGV_ERROR);
	if (find(si->si_addr, loader))
		exit(SIGSEGV_ERROR);
	//copiaza din fisier exact bucata de cod aferenta segmentului //
	void *pageAddress = mmap((void *)segment->vaddr + segment_offset, getpagesize(), PERM_R | PERM_W, MAP_FIXED | MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	copy_into(segment, segment_offset, pageAddress);
	addPage(pageAddress, loader);
	mprotect(pageAddress, getpagesize(), segment->perm);
}

int so_init_loader(void)
{
	struct sigaction sig;
	init(&loader);
	memset(&sig, 0, sizeof(sig));
	sig.sa_flags = SA_SIGINFO;
	sigemptyset(&sig.sa_mask);
	sig.sa_sigaction = signal_handler;
	sigaction(SIGSEGV, &sig, NULL);

	return -1;
}

int so_execute(char *path, char *argv[])
{
	exec_decriptor = open(path, O_RDONLY); //if this fails then the so_parse_exec will exit
	exec = so_parse_exec(path);
	if (!exec)
		return -1;

	so_start_exec(exec, argv);

	return -1;
}
