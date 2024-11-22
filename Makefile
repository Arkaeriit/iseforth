CFLAGS := -Wall -Wextra -g
LDFLAGS := -lseforth -lreadline
CC := gcc

all : iseforth

C_SRC := main.c dynamic_array.c output_text.c
C_HEADERS := dynamic_array.h output_text.h
C_OBJS := $(C_SRC:%.c=%.o)

iseforth : $(C_OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

%.o : %.c $(C_HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

clean :
	rm -f *.o
	rm -f iseforth

