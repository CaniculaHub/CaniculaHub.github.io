
NAME = demo
CROSS_COMPILE = arm-none-linux-gnueabi-
CFLAGS = -fno-builtin -nostdinc
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
OBJS = ${patsubst %.c, %.o, ${wildcard *.c}} 

all:  $(OBJS) start.o
	$(CC) $(CFLAGS) -c -o start.o start.S
	$(LD)  $(OBJS)  -T map.lds -o $(NAME).elf
	$(OBJCOPY)  -O binary  $(NAME).elf $(NAME).bin 
	$(OBJDUMP) -D $(NAME).elf > $(NAME).dis 
	

%.o: %.c
	$(CC) $(CFLAGS) -c -o  $@ $<  

clean:
	rm -rf $(OBJS) *.elf *.bin *.dis *.o


