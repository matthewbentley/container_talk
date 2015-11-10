#define _GNU_SOURCE
#include <sys/capability.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void update_map(int in, int out, int len, pid_t pid) {
    int fd;
    size_t map_len;
    char *mapping;
    char *map_file;

    map_len = snprintf(NULL, 0, "%d %d %d", in, out, len) + 1;
    mapping = malloc(map_len);
    sprintf(mapping, "%d %d %d", in, out, len);

    map_file = malloc(snprintf(NULL, 0, "/proc/%ld/uid_map", (long) pid) + 1);
    sprintf(map_file, "/proc/%ld/uid_map", (long) pid);

    fd = open(map_file, O_RDWR);

    write(fd, mapping, map_len);

    close(fd);
    free(mapping);
    free(map_file);
}


static int child(void *arg) {
    cap_t caps;

    for (;;) {
        printf("eUID = %ld;  eGID = %ld;  ",
                (long) geteuid(), (long) getegid());

        caps = cap_get_proc();
        printf("capabilities: %s\n", cap_to_text(caps, NULL));

        if (arg == NULL)
            break;

        sleep(5);
    }

    execlp("bash", "bash", (char *) NULL);
    return 0;
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

int main(int argc, char *argv[]) {
    pid_t pid;

    if (argc < 1)
        return 1;

    pid = clone(child, child_stack + STACK_SIZE, CLONE_NEWUSER | SIGCHLD, argv[1]);
    printf("Child pid: %ld\n", (long) pid);
    update_map(0, 1000, 1, pid);

    waitpid(pid, NULL, 0);

    return 0;
}
