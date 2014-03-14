TARGET_MODULE := Handle_func1
NAME_FILE := Handle_func
# If we are running by kernel building system
ifneq ($(KERNELRELEASE),)
	$(TARGET_MODULE)-objs := $(NAME_FILE).o
	obj-m := $(TARGET_MODULE).o

# If we running without kernel build system
else
	BUILDSYSTEM_DIR:=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)

all : 
# run kernel build system to make module
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules

clean:
# run kernel build system to cleanup in current directory
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) clean

load:
	insmod $(TARGET_MODULE).ko

unload:
	rmmod $(TARGET_MODULE)

endif
