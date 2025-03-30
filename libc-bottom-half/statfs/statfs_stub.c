#include <sys/statfs.h>
#include <sys/statvfs.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#ifndef TMPFS_MAGIC
#define TMPFS_MAGIC 0x01021994
#endif

#ifndef ST_RDONLY
#define ST_RDONLY 1
#endif
#ifndef ST_NOSUID
#define ST_NOSUID 2
#endif


static void fill_dummy_statfs(struct statfs *buf) {
    memset(buf, 0, sizeof(*buf));
    buf->f_type = TMPFS_MAGIC;
    buf->f_flags = 0;
    buf->f_bsize = 4096;
    buf->f_blocks = (1024ULL * 1024 * 1024 * 1024) / buf->f_bsize;
    buf->f_bfree = (800ULL * 1024 * 1024 * 1024) / buf->f_bsize;
    buf->f_bavail = buf->f_bfree;
    buf->f_files = 10000000;
    buf->f_ffree = 9000000;
    memset(&buf->f_fsid, 0, sizeof(buf->f_fsid));
    buf->f_namelen = 255;
    buf->f_frsize = buf->f_bsize;
}

int __statfs(const char *path, struct statfs *buf)
{
	(void)path;
	if (!buf) {
		errno = EFAULT;
		return -1;
	}
	fill_dummy_statfs(buf);
	return 0;
}

int __fstatfs(int fd, struct statfs *buf)
{
	(void)fd;
	if (!buf) {
		errno = EFAULT;
		return -1;
	}
	fill_dummy_statfs(buf);
	return 0;
}

int statfs(const char *path, struct statfs *buf) __attribute__((alias("__statfs")));
int fstatfs(int fd, struct statfs *buf) __attribute__((alias("__fstatfs")));


static void statfs_to_statvfs(const struct statfs *in, struct statvfs *out) {
    memset(out, 0, sizeof(*out));
    out->f_bsize = in->f_bsize;
    out->f_frsize = in->f_frsize;
    out->f_blocks = in->f_blocks;
    out->f_bfree = in->f_bfree;
    out->f_bavail = in->f_bavail;
    out->f_files = in->f_files;
    out->f_ffree = in->f_ffree;
    out->f_favail = in->f_ffree;
    out->f_fsid = 0;
    out->f_namemax = in->f_namelen;
    out->f_flag = 0;
    if (in->f_flags & ST_RDONLY) out->f_flag |= ST_RDONLY;
    if (in->f_flags & ST_NOSUID) out->f_flag |= ST_NOSUID;
}

int statvfs(const char *restrict path, struct statvfs *restrict buf)
{
    struct statfs stfs_buf;
    if (!buf) {
        errno = EFAULT;
        return -1;
    }
    if (statfs(path, &stfs_buf) < 0) {
        return -1;
    }
    statfs_to_statvfs(&stfs_buf, buf);
    return 0;
}

int fstatvfs(int fd, struct statvfs *buf)
{
    struct statfs stfs_buf;
    if (!buf) {
        errno = EFAULT;
        return -1;
    }
    if (fstatfs(fd, &stfs_buf) < 0) {
        return -1;
    }
    statfs_to_statvfs(&stfs_buf, buf);
    return 0;
}
