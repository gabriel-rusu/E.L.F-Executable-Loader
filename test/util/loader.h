/*
 * Operating System Executable Loader header
 *
 * 2018, Operating Systems
 */

#ifndef LOADER_H_
#define LOADER_H_

#if defined _WIN32
#if defined DLL_EXPORTS
#define FUNC_DECL_PREFIX __declspec(dllexport)
#else
#define FUNC_DECL_PREFIX __declspec(dllimport)
#endif /* DLL_EXPORTS */
#else
#define FUNC_DECL_PREFIX
#endif /* _WIN32 */

FUNC_DECL_PREFIX int so_init_loader(void);
FUNC_DECL_PREFIX int so_execute(char *path, char *argv[]);

#endif
