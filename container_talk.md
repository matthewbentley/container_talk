# OUTLINE

 - Into to containers
    - Buzzwords
    - Overview
 - Technology
    - Namespaces
        - Mount namespaces (CLONE_NEWNS)
        - UTS namespaces (CLONE_NEWUTS)
        - IPC namespaces (CLONE_NEWIPC)
        - PID namespaces (CLONE_NEWPID)
        - NET namespaces (CLONE_NEWNET)
        - User namespaces (CLONE_NEWUSER)
    - I will talk about UTS (hostname), mount, pid, network, and user namespaces
 - UTS
    - Used to isolate the hostname of a set of processes
    - `test_uts.c`
    - Bind mount to /root/utsns
    - `join_uts.c`
 - PID
    - Used to isolate PID groups
    - The first progess of a new PID group gets PID 1
    - Its parent gets PID 0

# TODO
1. Fill out mount and mount/pid outline
2. Write code for net namespace
3. Write code for user namespace
4. Write a FULL CONTAINER!!!
