#sudo su
#echo "56367"
make
insmod do_something.ko
#rmmod do_something
#insmod do_something.ko ds_mod_param=7
#rmmod do_something
dmesg |tail