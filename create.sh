#!/bin/bash

sudo mknod /dev/first c 91 0
sudo chmod a+r+w /dev/first
sudo mknod /dev/second c 92 0
sudo chmod a+r+w /dev/second
sudo mknod /dev/sign c 93 0
sudo chmod a+r+w /dev/sign
sudo mknod /dev/result c 94 0
sudo chmod a+r+w /dev/result