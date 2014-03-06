MODULE_NAME:=affl_mod
FILE_NAME1 :=load
FILE_NAME2 :=chrdev

$(MODULE_NAME)-objs := $(FILE_NAME1).o $(FILE_NAME2).o
obj-m := $(MODULE_NAME).o

KDIR  := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR)  M=$(PWD) clean