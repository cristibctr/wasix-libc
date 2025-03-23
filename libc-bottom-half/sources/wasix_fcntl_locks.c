#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/types.h>

/* Simple emulation of fcntl file locking for WASIX - stub implementation */
static int __wasix_fcntl_lock(int fd, int cmd, struct flock *lock) {
    if (!lock) {
        errno = EINVAL;
        return -1;
    }

    /* This is a stub implementation that pretends to succeed */
    if (cmd == F_GETLK) {
        /* Pretend no lock exists */
        lock->l_type = F_UNLCK;
    }

    /* For all other operations, just return success */
    return 0;
}

/* Implementation of fcntl that provides stub file locking support */
int fcntl(int fd, int cmd, ...) {
    int res = -1;
    va_list ap;
    va_start(ap, cmd);

    switch (cmd) {
        case F_GETLK:
        case F_SETLK:
        case F_SETLKW: {
            struct flock *lock = va_arg(ap, struct flock *);
            res = __wasix_fcntl_lock(fd, cmd, lock);
            break;
        }

        case F_GETFD:
        case F_SETFD:
        case F_GETFL:
        case F_SETFL:
        case F_DUPFD:
        case F_DUPFD_CLOEXEC:
            /* Handle standard fcntl operations here */
            /* For WASIX, we could implement these using other WASI functions */
            errno = ENOSYS; /* Not implemented yet */
            res = -1;
            break;

        default:
            errno = EINVAL;
            res = -1;
            break;
    }

    va_end(ap);
    return res;
}