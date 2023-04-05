
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

ifeq ($(KERNELRELEASE),)
	KERNELDIR = /home/dzq/workspace/qemu-training/build/vexpress/output/linux-4.20.17
	PWD =$(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
clean:
	rm -rf *.so *.o *.ko  .tmp_versions *.mod.c *.order *.symvers .*.cmd
else
	obj-m := mydrv.o
	mydrv-y := main.o i2c.o
endif
