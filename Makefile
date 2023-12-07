ifneq ($(KERNELRELEASE),)
include Kbuild
else
# normal makefile

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

ccflags-y := -I$(src)/inc

all: hello41.ko hello42.ko

hello41.ko:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

hello42.ko:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

endif

