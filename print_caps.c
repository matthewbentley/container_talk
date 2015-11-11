#define _GNU_SOURCE
#include <sys/capability.h>
#include <stdio.h>
#include <unistd.h>


int main() {
    cap_t caps;

    printf("After: eUID = %ld;  eGID = %ld;  ",
                (long) geteuid(), (long) getegid());

    caps = cap_get_proc();
    printf("capabilities: %s\n", cap_to_text(caps, NULL));
    cap_free(caps);

    return 0;
}
