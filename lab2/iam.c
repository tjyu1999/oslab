#include <errno.h>
#define __LIBRARY__
#include <stdio.h>
#include <unistd.h>

_syscall1(int, iam, const char *, name);

int main(int argc, char **argv) {
    iam(argv[1]);
    return 0;
}
