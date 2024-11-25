CFLAGS := -Wall -Wextra -g
LDFLAGS := -lseforth -lreadline
CC := gcc
PREFIX := /usr/local

all : iseforth

C_SRC := main.c dynamic_array.c output_text.c completion.c config.c config_literals.c history.c
C_HEADERS := dynamic_array.h output_text.h completion.h config.h history.h
C_OBJS := $(C_SRC:%.c=%.o)

iseforth : $(C_OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

%.o : %.c $(C_HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

%.c : %.frt
	name=$$(echo $< | sed s:.frt*::); \
		 echo "const char* $$name = " > $@
	cat $< | sed 's:\\:\\\\:g; s:( [^)]*): :g; s:\s\+\([^"]\): \1:g; s:\\ .*::;  s:":\\":g; s:^:":; s:$$:\\n":;' | grep -v '" \?\\n"'  >> $@
	echo ';' >> $@

install : iseforth
	cp $< $(PREFIX)/bin/iseforth

uninstall :
	rm -f $(PREFIX)/bin/iseforth

clean :
	rm -f *.o
	rm -f default_config.c
	rm -f iseforth

