obj-m += sleep.o

CC = gcc
cflags-y = -I.
DEPS =
OBJ = cat_nonblock.o

all: cat_nonblock
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cat_nonblock: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
