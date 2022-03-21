MOD = kdemo
MOD_DIR = $(shell pwd)

KERNELDIR = /usr/src/kernels/`uname -r`

# KBUILD_EXTRA_SYMBOLS += other module symbols file     eg:/root/driver_demo/Module.symvers
# export KBUILD_EXTRA_SYMBOLS

OBJECTS += start.o

obj-m := $(MOD).o
$(MOD)-y := $(OBJECTS)

all:
	make -C $(KERNELDIR) M=$(MOD_DIR)
clean:
	rm -f *.o *.mod.c *.order *.symvers *.ko
