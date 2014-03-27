MODULE_NAME:=FileInfo_mod
FILE_NAME1 :=FileInfo

$(MODULE_NAME)-objs := $(FILE_NAME1).o
obj-m := $(MODULE_NAME).o

KDIR  := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	rm -fr *.o *.mod.c *.order *.symvers .*.cmd .tmp_versions
clean:
	$(MAKE) -C $(KDIR)  M=$(PWD) clean