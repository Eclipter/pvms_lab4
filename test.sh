#!/bin/bash

sudo rmmod lab4
./remove.sh
./create.sh
make
sudo insmod lab4.ko
echo "28" > /dev/first
echo "0" > /dev/second
echo "/" > /dev/sign
cat /dev/result