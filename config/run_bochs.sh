#!/bin/bash

sudo /sbin/losetup /dev/loop8 ./config/flp.img
sudo bochs -f ./config/bochs.txt 
sudo /sbin/losetup -d /dev/loop8
