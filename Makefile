CC	:= clang
CFLAGS	:= -Wall -g3

module	:= lexer.c parser.c ir.c debug.c
src	:= main.c $(module)
obj	:= $(src:.c=.o)
bin	:= uopcc

$(bin): $(obj)
	echo $^
	$(CC) -o $@ $^

clean:
	rm -rf $(obj) $(bin) *.s a.out test.bin

test.bin: all_tests.c lexer_test.c parser_test.c $(module)
	$(CC) $^ -lcgreen -o $@

test: test.bin
	./$<
