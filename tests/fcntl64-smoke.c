#define _GNU_SOURCE
#include <fcntl.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

typedef int (*fcntl64_fn)(int fd, int command, ...);

int main(void)
{
	fcntl64_fn fcntl64 = (fcntl64_fn)dlsym(RTLD_DEFAULT, "fcntl64");
	char path[] = "/tmp/fcntl64-smoke-XXXXXX";
	struct flock lock = {
		.l_type = F_RDLCK,
		.l_whence = SEEK_SET,
	};
	int fd;
	int flags;

	if (fcntl64 == NULL)
		return 1;
	fd = mkstemp(path);
	if (fd < 0)
		return 2;
	if (fcntl64(fd, F_SETFD, FD_CLOEXEC) < 0)
		return 3;
	flags = fcntl64(fd, F_GETFD);
	if (flags < 0 || (flags & FD_CLOEXEC) == 0)
		return 4;
	if (fcntl64(fd, F_GETLK, &lock) < 0)
		return 5;

	close(fd);
	unlink(path);
	return 0;
}

