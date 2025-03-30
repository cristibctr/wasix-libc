#include <sys/mman.h>
#include <errno.h>

int madvise(void *addr, size_t len, int advice) __attribute__((weak, alias("posix_madvise")));

int posix_madvise(void *addr, size_t len, int advice) {
    // WASI has no mechanism to control memory advisory information.
    // Silently ignore the request, returning success as required by POSIX
    // for advisory functions when the advice is unsupported or ignored.
    (void)addr;
    (void)len;
    (void)advice;
    return 0;
}