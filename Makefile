CFLAGS := -Wall -Wextra -g
LDFLAGS := -lseforth
CC := gcc

all : iseforth

C_SRC := main.c dynamic_array.c
C_OBJS := $(C_SRC:%.c=%.o)

iseforth : $(C_OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

%.o : %.c
	$(CC) -c $< $(CFLAGS) -o $@

clean :
	rm -f *.o
	rm -f iseforth

