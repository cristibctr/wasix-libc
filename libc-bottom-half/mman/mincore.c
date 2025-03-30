#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

int mincore(void *addr, size_t len, unsigned char *vec) {
    // WASI has no concept of page residency in the traditional sense (like swapping).
    // WebAssembly linear memory is generally considered "resident".
    // We pretend all pages in the specified range are resident, as this is
    // often less disruptive than returning ENOSYS.

    if (len == 0) {
        return 0;
    }

    long page_size_long = sysconf(_SC_PAGESIZE);
    if (page_size_long <= 0) {
         errno = EINVAL;
         return -1;
    }

    size_t page_size = (size_t)page_size_long;
    if ((uintptr_t)addr % page_size != 0) {
         errno = EINVAL;
         return -1;
    }

    uintptr_t start_addr = (uintptr_t)addr;
    uintptr_t end_addr = start_addr + len;
    size_t num_pages = (len + page_size - 1) / page_size;
    memset(vec, 1, num_pages);

    return 0;
}