#!/bin/bash 

export PATH=$PATH:/usr/local/sbin

sudo losetup -P /dev/loop13 ./config/flp.img
sudo mount /dev/loop13 /mnt/kernel_u
sudo cp -r ./bin/kernel.elf /mnt/kernel_u
sudo cp -r ./bin/initrd.img /mnt/kernel_u
sudo umount /dev/loop13
sudo losetup -d /dev/loop13
