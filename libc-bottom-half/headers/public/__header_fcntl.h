#ifndef __wasilibc___header_fcntl_h
#define __wasilibc___header_fcntl_h

#include <wasi/api.h>
#include <__seek.h>
#include <__mode_t.h>

#define O_APPEND __WASI_FDFLAGS_APPEND
#define O_DSYNC __WASI_FDFLAGS_DSYNC
#define O_NONBLOCK __WASI_FDFLAGS_NONBLOCK
#define O_RSYNC __WASI_FDFLAGS_RSYNC
#define O_SYNC __WASI_FDFLAGS_SYNC
#define O_CREAT (__WASI_OFLAGS_CREAT << 12)
#define O_DIRECTORY (__WASI_OFLAGS_DIRECTORY << 12)
#define O_EXCL (__WASI_OFLAGS_EXCL << 12)
#define O_TRUNC (__WASI_OFLAGS_TRUNC << 12)

#define O_NOFOLLOW (0x01000000)
#define O_EXEC     (0x02000000)
#define O_RDONLY   (0x04000000)
#define O_SEARCH   (0x08000000)
#define O_WRONLY   (0x10000000)

#define O_CLOEXEC (__WASI_FDFLAGSEXT_CLOEXEC << 30)

/*
 * O_TTY_INIT is defined to be zero, meaning that WASI implementations are
 * expected to always initialize a terminal the first time it's opened.
 */
#define O_TTY_INIT (0)

#define O_NOCTTY   (0)

#define O_RDWR (O_RDONLY | O_WRONLY)
#define O_ACCMODE (O_EXEC | O_RDWR | O_SEARCH)

#define POSIX_FADV_DONTNEED __WASI_ADVICE_DONTNEED
#define POSIX_FADV_NOREUSE __WASI_ADVICE_NOREUSE
#define POSIX_FADV_NORMAL __WASI_ADVICE_NORMAL
#define POSIX_FADV_RANDOM __WASI_ADVICE_RANDOM
#define POSIX_FADV_SEQUENTIAL __WASI_ADVICE_SEQUENTIAL
#define POSIX_FADV_WILLNEED __WASI_ADVICE_WILLNEED

#define F_GETFD (1)
#define F_SETFD (2)
#define F_GETFL (3)
#define F_SETFL (4)
#define F_DUPFD (5)
#define F_DUPFD_CLOEXEC (6)

#define FD_CLOEXEC (1)

#define AT_EACCESS          (0x0)
#define AT_SYMLINK_NOFOLLOW (0x1)
#define AT_SYMLINK_FOLLOW   (0x2)
#define AT_REMOVEDIR        (0x4)

#define AT_FDCWD (-2)

/* File locking types for fcntl() */
#define F_RDLCK     0
#define F_WRLCK     1
#define F_UNLCK     2

/* File locking commands for fcntl() */
#define F_GETLK     7
#define F_SETLK     8
#define F_SETLKW    9

/* Define _POSIX_MAPPED_FILES which is required by some applications */
#ifndef _POSIX_MAPPED_FILES
#define _POSIX_MAPPED_FILES 200809L
#endif

#endif
