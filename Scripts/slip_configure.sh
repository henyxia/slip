#!/bin/bash

slattach -L -s 9600 -p slip /dev/ttyUSB1 &
sleep 1
ifconfig sl0 192.168.0.1 dstaddr 192.168.1.4 mtu 1500 

