obj-m += sleep.o

CC = gcc
cflags-y = -I.
DEPS =
OBJ = cat_nonblock.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ioctl: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

all: sleep
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
