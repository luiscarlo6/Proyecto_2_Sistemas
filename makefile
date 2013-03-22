# Makefile 
# --O1 -O3 -Wfatal-errors -Werror
CC = gcc
CFLAGS1 = -ansi -Wabi -g -ggdb -gstabs -Wall -Wextra -pedantic -pedantic-errors \
	  -Wswitch-default -Wswitch-enum -Wmain \
	-Wmissing-include-dirs -Wmissing-declarations -Wunreachable-code \
	-Winline -Wfloat-equal -Wundef -Wcast-align -Wredundant-decls \
	-Winit-self -Wshadow -Wformat-y2k -Wno-format-extra-args -Wformat=2 \
	-Wno-format-zero-length -Wformat-nonliteral -Wformat-security 
OBJS  = main.o
PROG  = Main


all : $(PROG) $(PROG1)

$(PROG): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(PROG)
		
		
%.o: %.c  
	$(CC) $(CFLAGS) -c $<

.PHONY : clean

clean:
	$(RM) $(PROG) $(OBJS) *~ *.o
