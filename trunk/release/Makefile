NAME_FILE1 := affl_main
NAME_FILE2 := affl_process
NAME_FILE3 := affl_char_dev
TARGET_MODULE := affl_mod
# If we are running by kernel building system
ifneq ($(KERNELRELEASE),)
	$(TARGET_MODULE)-objs := $(NAME_FILE1).o $(NAME_FILE2).o $(NAME_FILE3).o
	obj-m := $(TARGET_MODULE).o

# If we running without kernel build system
else
	BUILDSYSTEM_DIR:=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)

all : 
# run kernel build system to make module
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules
	rm *o *.mod.c *.order *.symvers *.cmd
clean:
# run kernel build system to cleanup in current directory
	rm *.ko

load:
	insmod $(TARGET_MODULE).ko

unload:
	rmmod $(TARGET_MODULE)

endif
