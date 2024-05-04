CC_FLAGS_FPU := -mhard-float

CFLAGS_moon.o += $(CC_FLAGS_FPU)
CFLAGS_REMOVE_moon.o += $(CC_FLAGS_NO_FPU)

obj-m += moonphase.o
moonphase-objs+= moon.o moonphase_main.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
