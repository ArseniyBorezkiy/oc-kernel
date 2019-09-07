#!/bin/bash 

export PATH=$PATH:/usr/local/sbin

sudo losetup -P /dev/loop8 flp.img
sudo mount /dev/loop8 /mnt/kernel_u
sudo cp -r ./bin/kernel.elf /mnt/kernel_u
sudo umount /dev/loop8
sudo losetup -d /dev/loop8
