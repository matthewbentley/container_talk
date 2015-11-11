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
    - The first process of a new PID group gets PID 1
    - Its parent gets PID 0
    - `test_pid.c`
    - Problems with this, leading to:
 - Mount
    - `test_pid_proc.c`
    - Add uts
    - `test_pid_proc_uts.c`
 - Net
    - Isolates network stacks
    - Includes interfaces, routing tables, and iptables
    - Can create with `ip netns add name`, available in /run/netns/
    - Then join with setns
    - `test_net.c`
    - `test_ip_proc_uts_net.c`
 - Brief interlude on networking
    - See `setup_nat.sh`
    - I won’t cover bridging or nat, but this requires you to have a bridge 
    called virbr0 with nat set up already
 - chroot
    - Finish a full container (debian based) with chroot
    - `test_mount.c`
    - `test_pid_proc_uts_net_newroot.c`
 - user
    - I couldn’t get these to work properly
    - Should allow better access control, etc
 - Capabilities
    - Didn’t get into this, but it exists
 - cgroups
    - This also exists

# TODO
SLIDES
