prefix:=$(shell xeno-config --prefix)
ARCH=arm
EXEC=gpio_test
CC=gcc
OBJS=main.o gpio.o
CFLAGS:=-Wall $(shell xeno-config --skin native --cflags) $(shell xeno-config --skin posix --cflags) $(shell xeno-config --skin rtdm --cflags)
LDFLAGS:=$(shell xeno-config --skin native --ldflags) $(shell xeno-config --skin posix --ldflags) $(shell xeno-config --skin rtdm --ldflags) -lm
.SUFFIXES: .c .o

$(EXEC): $(OBJS)
	$(CC) -v $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS) 

.c.o:
	$(CC) -v $(CFLAGS) $(LDFLAGS) -c $<

clean:
	$(RM) $(EXEC) $(OBJS)





