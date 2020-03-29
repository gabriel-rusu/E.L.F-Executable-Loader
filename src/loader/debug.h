#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>

#if defined DEBUG
#define dprintf(format, ...)                                    \
	fprintf(stderr, " [%s(), %s:%u] " format,               \
		__FUNCTION__, __FILE__, __LINE__,               \
		##__VA_ARGS__)
#else
#define dprintf(format, ...)                                    \
	do {} while (0)
#endif

#endif
