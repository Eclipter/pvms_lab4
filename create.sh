#!/bin/bash

sudo mknod /dev/first c 86 0
sudo chmod a+r+w /dev/first
sudo mknod /dev/second c 87 0
sudo chmod a+r+w /dev/second
sudo mknod /dev/sign c 88 0
sudo chmod a+r+w /dev/sign
sudo mknod /dev/result c 89 0
sudo chmod a+r+w /dev/result