#ifndef MDBX_COMPATIBILITY_H
#define MDBX_COMPATIBILITY_H

/* Define _POSIX_MAPPED_FILES which is required by mdbx.c */
#ifndef _POSIX_MAPPED_FILES
#define _POSIX_MAPPED_FILES 200809L
#endif

/* Fix for format warning in line 7165 */
#ifdef MDBX_PGL_LIMIT
#undef MDBX_PGL_LIMIT
#define MDBX_PGL_LIMIT ((unsigned int)(MAX_MAPSIZE32 / MIN_PAGESIZE))
#endif

/*
 * File locking constants should now be defined in the WASIX libc,
 * but we include them here just in case
 */
#ifndef F_WRLCK
#define F_WRLCK 1
#endif

#ifndef F_RDLCK
#define F_RDLCK 0
#endif

#ifndef F_UNLCK
#define F_UNLCK 2
#endif

#ifndef F_GETLK
#define F_GETLK 7
#endif

#ifndef F_SETLK
#define F_SETLK 8
#endif

#ifndef F_SETLKW
#define F_SETLKW 9
#endif

/* Ensure S_IFSOCK and S_IFIFO have different values */
#if defined(S_IFSOCK) && defined(S_IFIFO) && S_IFSOCK == S_IFIFO
#undef S_IFSOCK
#define S_IFSOCK (0xd000)  /* Different from S_IFIFO which is 0xc000 */
#endif

/*
 * The fcntl implementation in WASIX libc provides a stub file locking
 * implementation that will allow mdbx.c to compile and run even though
 * the file locking operations don't actually lock anything.
 */

#endif /* MDBX_COMPATIBILITY_H */