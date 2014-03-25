sudo make
sudo rm -fr *.o *.mod.c *.order *.symvers .*.cmd .tmp_versions
insmod affl_mod.ko
rmmod affl_mod
dmesg |tail