#define _GNU_SOURCE
#include <sys/utsname.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd;
    struct utsname uts;

    if (argc < 2)
        return 1;

    fd = open(argv[1], O_RDONLY);

    setns(fd, 0);

    uname(&uts);

    printf("hostname of this (%ld): %s\n", (long) getpid(), uts.nodename);

    return 0;
}
