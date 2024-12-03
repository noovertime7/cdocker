CC = gcc
CFLAGS = -g -Wall -Wextra -D_GNU_SOURCE
SRC = main.c include/container.c include/tools.c include/namespace.c include/overlayfs.c
OBJ = $(SRC:.c=.o)
EXEC = cdocker

all: $(EXEC)
	chmod u+x $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lutil

clean:
	rm -f $(OBJ) $(EXEC)

debug:
	sudo gdbserver :1234 ./$(EXEC) --debug-mode

.PHONY: all clean m1 m2 unmount

m1:
	sudo mount -t overlay overlay -o lowerdir=rootfs/lowdir,upperdir=rootfs/upperdir,workdir=rootfs/workdir rootfs/merged

m2:
	sudo mount -t overlay overlay -o lowerdir=rootfs/lowdir:rootfs/lowdir2,upperdir=rootfs/upperdir,workdir=rootfs/workdir rootfs/merged

umount:
	sudo umount rootfs/merged	