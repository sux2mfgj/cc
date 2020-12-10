CC	:= cc
CFLAGS	:= -Wall -ggdb3

src	:= main.c lexer.c parser.c ir.c
obj	:= $(src:.c=.o)
bin	:= uopcc

$(bin): $(obj)
	echo $^
	$(CC) -o $@ $^

clean:
	rm -rf $(obj) $(bin) *.s a.out

