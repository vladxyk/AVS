#!/bin/bash
echo "Date: `date`"
echo "Name: `whoami`"
echo "Domain name: `hostname`"
#
#
echo "CPU:"
#str1=$(lscpu)
lscpu | grep 'Model name' | awk '{print "  • " $0}'
lscpu | grep 'Arch' | awk '{print "  • " $0}'
lscpu | grep 'CPU M' | awk '{print "  • " $0}'
lscpu | grep 'CPU(s):' | awk '{print "  • " $0}'
lscpu | grep 'Th' | awk '{print "  • " $0}'
#echo ram
echo "RAM: "
#str2=$(free -h)
#echo "  • "${str2:14:5} = ${str2:94:5}
free -h | grep 'Mem' | awk '{print "  • total = " $2}'
free -h | grep 'Mem' | awk '{print "  • available = " $7}'
echo "SSD: "
df -h | grep 'C' | awk '{print "  • total = " $2}'
df -h | grep 'C' | awk '{print "  • available = " $4}'
df -h | grep 'root' | awk '{print "  • mounted on / = " $2}'
free -h | grep 'Swap' | awk '{print "  • swap total = " $2}'
free -h | grep 'Swap' | awk '{print "  • swap available = " $4}'
#df -h
echo "Network interface: "
#str4=$(ifconfig -a)
echo "name:"
ifconfig -a | cut -c 1-8 | sort | uniq -u | awk -F: '{print "  • " $1}'
echo "Mac address:"
ifconfig -a | grep ether | awk '{print "  • " $2}'
echo "ip address:"
ifconfig -a | grep 'inet ' | awk '{print "  • " $2}'
