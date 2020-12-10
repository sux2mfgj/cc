CC	:= cc
CFLAGS	:= -Wall -ggdb3

module	:= lexer.c parser.c ir.c
src	:= main.c $(module)
obj	:= $(src:.c=.o)
bin	:= uopcc

$(bin): $(obj)
	echo $^
	$(CC) -o $@ $^

clean:
	rm -rf $(obj) $(bin) *.s a.out

test: all_tests.c lexer_test.c parser_test.c $(module)
	$(CC) $^ -lcgreen -o $@
	./$@

