obj-m += procfs-1.o
obj-m += procfs-2.o
obj-m += procfs-3.o
obj-m += procfs-4.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
