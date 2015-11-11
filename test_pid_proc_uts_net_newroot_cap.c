#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <sys/capability.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>

struct child_args {
    char *hostname;
    char *chroot_to;
};

static int child(void *arg) {
    struct child_args *cargs = (struct child_args *) arg;

    sethostname(cargs->hostname, strlen(cargs->hostname));

    printf("My PID: %ld\n", (long) getpid());
    printf("My PPID: %ld\n", (long) getppid());

    mount("", "/", "dontcare", MS_REC|MS_SLAVE, NULL);
    chroot(cargs->chroot_to);
    chdir("/");

    umount("/proc");
    mount("proc", "/proc", "proc", 0, NULL);

    execlp("/bin/bash", "/bin/bash", (char *) NULL);

    return 0;
}

#define CLEAR_LEN 25
#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

int main(int argc, char *argv[]) {
    pid_t child_pid;
    int fd, i, clone_flags;
    struct child_args cargs;

    cap_flag_t to_clear[CLEAR_LEN] = {
        CAP_SYS_ADMIN,
        CAP_MAC_ADMIN,
        CAP_MAC_OVERRIDE,
        CAP_DAC_OVERRIDE,
        CAP_AUDIT_CONTROL,
        CAP_AUDIT_WRITE,
        CAP_BLOCK_SUSPEND,
        CAP_MKNOD,
        CAP_NET_ADMIN,
        CAP_SYS_BOOT,
        CAP_SYS_MODULE,
        CAP_SYS_NICE,
        CAP_SYS_TIME,
        CAP_SETPCAP,
    };

    for(i=0; i<CLEAR_LEN; i++) {
        if(to_clear[i]) {
            printf("Dropping %d, result: %d\n",
                    to_clear[i], prctl(PR_CAPBSET_DROP, to_clear[i]));
        }
    }

    clone_flags = CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD;

    if (argc < 4)
        return 1;

    cargs.hostname = argv[1];
    cargs.chroot_to = argv[3];

    if (strlen(argv[2]) > 0) {
        fd = open(argv[2], O_RDONLY);
        setns(fd, 0);
    } else {
        clone_flags |= CLONE_NEWNET;
    }

    child_pid = clone(
            child,
            child_stack + STACK_SIZE,
            clone_flags, &cargs);

    printf("PID of child: %ld\n", (long) child_pid);

    waitpid(child_pid, NULL, 0);

    return 0;
}
