MODULE_NAME:=mod_mod
FILE_NAME := mod

$(MODULE_NAME)-objs := $(FILE_NAME).o
obj-m := $(MODULE_NAME).o

KDIR  := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	rm *.o *.mod.c *.order *.symvers 
clean:
	$(MAKE) -C $(KDIR)  M=$(PWD) clean