CC = $(CROSS_COMPILE)gcc
DEBUG =  -Wall
CFLAGS += $(DEBUG)
PROGS = ${patsubst %.c, %, ${wildcard *.c}} 
all : $(PROGS)
% : %.c
	$(CC)  $(CFLAGS)  $< -o $@ 
.PHONY:clean
clean : uninstall
	- rm -f *.o

