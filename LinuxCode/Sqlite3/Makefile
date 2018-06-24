CC =gcc
DEBUG = -Wall
CFLAGS += $(DEBUG)
PROGS = ${patsubst %.c, %, ${wildcard *.c}} 
all : $(PROGS)
% : %.c
	$(CC)  $(CFLAGS)  $< -o $@ -lsqlite3
.PHONY:clean
clean : uninstall
	- rm -f *.o

