CFLAGS := -Wall -Wextra -g
LDFLAGS := -lseforth

all : iseforth

C_SRC := main.c dynamic_array.c
C_OBJS := $(C_SRC:%.c=%.o)

iseforth : $(C_OBJS)
	$(CC) $< $(LDFLAGS) -o $@

%.c : %.o
	$(CC) -c $< (CFLAGS) $@

clean :
	rm -f *.o
	rm -f iseforth

