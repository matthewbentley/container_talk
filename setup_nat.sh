#!/bin/bash

# This sets up a bridge, and puts one side of it in a network namespace.
# Requires dhcpcd be installed
# Before this script, you should bring up lo in the namespace
# This assumes you already have a bridge set up with nat and a dhcp server
#  called virbr0
# See here: http://docs.docker.com/v1.7/articles/networking/ for more

# $1 should be the network namespace, $2 should be the external nic name, 
#  $3 should be the internal nic name

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

ip link add $2 type veth peer name $3
brctl addif virbr0 $2
ip link set $2 up

ip link set $3 netns $1
ip netns exec $1 ip link set $3 up
ip netns exec $1 dhcpcd $3
