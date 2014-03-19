MODULE_NAME:=affl_mod
FILE_NAME1 :=affl_main
FILE_NAME2 :=affl_chrdev
FILE_NAME3 :=affl_process

$(MODULE_NAME)-objs := $(FILE_NAME1).o $(FILE_NAME2).o $(FILE_NAME3).o
obj-m := $(MODULE_NAME).o

KDIR  := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	rm -fr *.o *.mod.c *.order *.symvers .*.cmd .tmp_versions
clean:
	$(MAKE) -C $(KDIR)  M=$(PWD) clean