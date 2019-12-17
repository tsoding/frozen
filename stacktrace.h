#ifndef STACKTRACE_H_
#define STACKTRACE_H_

#include <stdlib.h>
#include <stdio.h>
#if defined(__GNUC__) && defined(__linux__)
#include <execinfo.h>
#include <unistd.h>
#define N 100
#endif

#ifndef NDEBUG
#define trace_assert(condition) (void)((condition) || (__trace_assert(__FILE__, __LINE__, __func__, #condition), 0))
#else
#define trace_assert(condition) (void)(condition)
#endif

static inline
void print_stacktrace(void)
{
#if defined(__GNUC__) && defined(__linux__)
    void *array[N];
    int size;

    size = backtrace(array, N);

    if (size <= 0) {
        return;
    }

    fprintf(stderr, "Stacktrace: \n");
    backtrace_symbols_fd(array + 1, size - 1, STDERR_FILENO);
#endif
}

static inline
void __trace_assert(const char *file, int line, const char *function, const char *message)
{
    fprintf(
        stderr,
        "%s:%d: %s: Assertion `%s' failed\n",
        file, line,
        function,
        message);
    print_stacktrace();
    abort();
}

#endif  // STACKTRACE_H_
