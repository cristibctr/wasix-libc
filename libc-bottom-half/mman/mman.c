// Userspace emulation of mmap and munmap. Restrictions apply.
//
// This is meant to be complete enough to be compatible with code that uses
// mmap for simple file I/O. It just allocates memory with malloc and reads
// and writes data with pread and pwrite.

#ifdef __wasilibc_unmodified_upstream
#define _WASI_EMULATED_MMAN
#else
#define _WASI_EMULATED_MMAN 1
#endif
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>

struct map {
    int prot;
    int flags;
    off_t offset;
    size_t length;
    int fd;
};

void *mmap(void *addr, size_t length, int prot, int flags,
           int fd, off_t offset) {
    // Check for unsupported flags.
    if ((flags & (MAP_PRIVATE | MAP_SHARED)) == 0 ||
        (flags & MAP_FIXED) != 0 ||
#ifdef MAP_SHARED_VALIDATE
        (flags & MAP_SHARED_VALIDATE) == MAP_SHARED_VALIDATE ||
#endif
#ifdef MAP_NORESERVE
        (flags & MAP_NORESERVE) != 0 ||
#endif
#ifdef MAP_GROWSDOWN
        (flags & MAP_GROWSDOWN) != 0 ||
#endif
#ifdef MAP_HUGETLB
        (flags & MAP_HUGETLB) != 0 ||
#endif
#ifdef MAP_FIXED_NOREPLACE
        (flags & MAP_FIXED_NOREPLACE) != 0 ||
#endif
        0)
    {
        errno = EINVAL;
        return MAP_FAILED;
    }

    // Check for unsupported protection requests.
    if (prot == PROT_NONE ||
#ifdef PROT_EXEC
        (prot & PROT_EXEC) != 0 ||
#endif
        0)
    {
        errno = EINVAL;
        return MAP_FAILED;
    }

    //  To be consistent with POSIX.
    if (length == 0) {
        errno = EINVAL;
        return MAP_FAILED;
    }

    // Check for integer overflow.
    size_t buf_len = 0;
    if (__builtin_add_overflow(length, sizeof(struct map), &buf_len)) {
        errno = ENOMEM;
        return MAP_FAILED;
    }

    // Allocate the memory.
    struct map *map = malloc(buf_len);
    if (!map) {
        errno = ENOMEM;
        return MAP_FAILED;
    }

    // Initialize the header.
    map->prot = prot;
    map->flags = flags;
    map->offset = offset;
    map->length = length;

    // Initialize the main memory buffer, either with the contents of a file,
    // or with zeros.
    addr = map + 1;
    if ((flags & MAP_ANON) == 0) {
        int new_fd = dup(fd);

        if (new_fd < 0) {
            errno = EINVAL;
            free(map);

            return NULL;
        }

        map->fd = new_fd;

        char *body = (char *)addr;
        while (length > 0) {
            const ssize_t nread = pread(fd, body, length, offset);
            if (nread < 0) {
                if (errno == EINTR)
                    continue;
                return MAP_FAILED;
            }
            if (nread == 0)
                break;
            length -= (size_t)nread;
            offset += (size_t)nread;
            body += (size_t)nread;
        }
    } else {
        map->fd = -1;
        memset(addr, 0, length);
    }

    return addr;
}

int munmap(void *addr, size_t length) {
    struct map *map = (struct map *)addr - 1;

    // We don't support partial munmapping.
    if (map->length != length) {
        errno = EINVAL;
        return -1;
    }

    // Write the data back to the backing file and close
    // the file handle
    if (map->fd > 0) {
        if ((map->prot & PROT_WRITE) != 0) {
            msync(addr, length, MS_SYNC);
        }

        close(map->fd);
    }

    // Release the memory.
    free(map);

    // Success!
    return 0;
}

int msync (void *addr, size_t length, int flags) {
    struct map *map = (struct map *)addr - 1;
    size_t map_flags = map->flags;
    off_t map_offset = map->offset;
    size_t map_length = map->length;
    int fd = map->fd;

    if (length > map_length) {
        errno = EINVAL;
        return -1;
    }

    if ((map->prot & PROT_WRITE) != 0) {
        errno = EINVAL;
        return -1;
    }

    if ((map_flags & MAP_ANON) == 0) {
        char *body = (char *)addr;

        while (length > 0) {
            const ssize_t nwrite = pwrite(fd, body, length, map_offset);

            if (nwrite > 0) {
                length -= (size_t)nwrite;
                map_offset += (size_t)nwrite;
                body += (size_t)nwrite;
            } else if (errno == EINTR) {
                continue;
            } else {
                return -1;
            }
        }
    }

    return 0;
}
