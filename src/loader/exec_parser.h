/*
 * Executable Parser Header
 *
 * 2018, Operating Systems
 */

#ifndef SO_EXEC_PARSER_H_
#define SO_EXEC_PARSER_H_

#include <stdint.h>

#define ALIGN_DOWN(v, a) ((v) & ~((a) - 1))
#define ALIGN_UP(v, a) (((v) + ((a) - 1)) & ~((a)-1))

#define PERM_R 0x1
#define PERM_W 0x2
#define PERM_X 0x4

typedef struct so_seg {
	/* virtual address */
	uintptr_t vaddr;
	/* size inside the executable file */
	unsigned int file_size;
	/* size in memory (can be larger than file_size */
	unsigned int mem_size;
	/* offset in file */
	unsigned int offset;
	/* permissions */
	unsigned int perm;
	/* custom data */
	void *data;
} so_seg_t;

typedef struct so_exec {
	/* base adress */
	uintptr_t base_addr;
	/* address of entry point */
	uintptr_t entry;
	/* number of segments */
	int segments_no;
	/* array of segments */
	so_seg_t *segments;
} so_exec_t;

/* parse an executable file */
so_exec_t *so_parse_exec(char *path);

/*
 * start an executable file, previously parsed in a so_exec_t structure
 * (jumps to the executable's entry point)
 */
void so_start_exec(so_exec_t *exec, char *argv[]);

#endif /* SO_EXEC_PARSER_H_ */
