MODULE_NAME:=do_something_mod
FILE_NAME := do_something

$(MODULE_NAME)-objs := $(FILE_NAME).o
obj-m := $(MODULE_NAME).o

KDIR  := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR)  M=$(PWD) clean