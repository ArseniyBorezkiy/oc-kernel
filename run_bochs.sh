#!/bin/bash

sudo /sbin/losetup /dev/loop8 flp.img
sudo bochs -f bochs.txt 
sudo /sbin/losetup -d /dev/loop8
