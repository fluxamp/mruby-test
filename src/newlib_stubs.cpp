
// based on the newlib documentation, chapter 12.1
// see https://sourceware.org/newlib/libc.html#Stubs

#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <libopencmsis/core_cm3.h>

#undef errno
extern int errno;

#define __ASM __asm
#include "core_cmFunc.h"

extern "C" {
    char *__env[1] = { 0 };
    char **environ = __env;

    void _exit(int status)
    {
        while (true) {};
    }

    int __attribute__((weak)) _close(int file)
    {
        return -1;
    }

    int __attribute__((weak)) _fork(void)
    {
        errno = EAGAIN;
        return -1;
    }

    int __attribute__((weak)) _fstat(int file, struct stat *st)
    {
        st->st_mode = S_IFCHR;
        return 0;
    }

    int __attribute__((weak)) _getpid(void)
    {
        return 1;
    }

    int __attribute__((weak)) _isatty(int file)
    {
        return 1;
    }

    int __attribute__((weak)) _kill(int pid, int sig)
    {
        errno = EINVAL;
        return -1;
    }

    int __attribute__((weak)) _link(char *_old, char *_new)
    {
        errno = EMLINK;
        return -1;
    }

    int __attribute__((weak)) _lseek(int file, int ptr, int dir)
    {
        return 0;
    }

    int __attribute__((weak)) _open(const char *name, int flags, int mode)
    {
        return -1;
    }

    int __attribute__((weak)) _read(int file, char *ptr, int len)
    {
        return 0;
    }

    int __attribute__((weak)) _write(int file, char *ptr, int len)
    {
        return 0;
    }

    caddr_t __attribute__((weak)) _sbrk(int incr)
    {
        extern char _ebss; // defined in the linker script
        static char *heap_end;
        char *prev_heap_end;

        if (heap_end == 0)
        {
            heap_end = &_ebss;
        }
        prev_heap_end = heap_end;

        char *stack_ptr = (char *)__get_MSP();
        if (heap_end + incr > stack_ptr)
        {
            _exit(0);
        }

        heap_end += incr;
        return (caddr_t)prev_heap_end;
    }

    int __attribute__((weak)) _stat(const char *file, struct stat *st)
    {
        st->st_mode = S_IFCHR;
        return 0;
    }

    clock_t __attribute__((weak)) _times(struct tms *buf)
    {
        return -1;
    }

    int __attribute__((weak)) _unlink(char *name)
    {
        errno = ENOENT;
        return -1;
    }

    int __attribute__((weak)) _wait(int *status)
    {
        errno = ECHILD;
        return -1;
    }

    void __attribute__((weak)) _fini(void)
    {
        while(true) {};
    }
}