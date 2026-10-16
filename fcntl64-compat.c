#define _GNU_SOURCE
#include <fcntl.h>
#include <stdarg.h>

static int has_no_argument(int command)
{
	switch (command) {
	case F_GETFD:
	case F_GETFL:
	case F_GETOWN:
#ifdef F_GETSIG
	case F_GETSIG:
#endif
#ifdef F_GETLEASE
	case F_GETLEASE:
#endif
#ifdef F_GETPIPE_SZ
	case F_GETPIPE_SZ:
#endif
#ifdef F_GET_SEALS
	case F_GET_SEALS:
#endif
		return 1;
	default:
		return 0;
	}
}

static int has_integer_argument(int command)
{
	switch (command) {
	case F_DUPFD:
#ifdef F_DUPFD_CLOEXEC
	case F_DUPFD_CLOEXEC:
#endif
	case F_SETFD:
	case F_SETFL:
	case F_SETOWN:
#ifdef F_SETSIG
	case F_SETSIG:
#endif
#ifdef F_SETLEASE
	case F_SETLEASE:
#endif
#ifdef F_NOTIFY
	case F_NOTIFY:
#endif
#ifdef F_SETPIPE_SZ
	case F_SETPIPE_SZ:
#endif
#ifdef F_ADD_SEALS
	case F_ADD_SEALS:
#endif
		return 1;
	default:
		return 0;
	}
}

/* fcntl and fcntl64 share the same ABI on 64-bit musl. */
int fcntl64(int fd, int command, ...)
{
	va_list arguments;
	int result;

	va_start(arguments, command);
	if (has_no_argument(command)) {
		result = fcntl(fd, command);
	} else if (has_integer_argument(command)) {
		result = fcntl(fd, command, va_arg(arguments, int));
	} else {
		result = fcntl(fd, command, va_arg(arguments, void *));
	}
	va_end(arguments);
	return result;
}
