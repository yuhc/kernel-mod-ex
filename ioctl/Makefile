obj-m += chardev.o

CC = gcc
cflags-y = -I.
DEPS = chardev.h
OBJ = ioctl.o

all: ioctl
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ioctl: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
