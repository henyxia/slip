#!/bin/bash

slattach -L -s 9600 -p slip /dev/ttyUSB0 &
sleep 1
#ifconfig sl0 192.168.0.1 dstaddr 192.168.1.4 mtu 1500
ip link set sl0 up
ip address add dev sl0 172.26.79.200 peer 172.26.79.201
echo 1 > /proc/sys/net/ipv4/ip_forward
echo 1 > /proc/sys/net/ipv4/conf/all/proxy_arp
ip route add 172.26.79.201/32 dev sl0
socat UDP-RECVFROM:12345,fork,so-bindtodevice=sl0 UDP-DATAGRAM:172.26.79.255:12345,sp=54321,reuseaadr,broadcast &
