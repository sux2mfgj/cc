CC	:= clang
CFLAGS	:= -Wall -g3

module	:= lexer.c parser.c ir.c debug.c
headers := lexer.h parser.h ir.h
src	:= main.c $(module)
obj	:= $(src:.c=.o)
bin	:= uopcc

$(bin): $(obj) $(headers)
	echo $^
	$(CC) -o $@ $(obj)

clean:
	rm -rf $(obj) $(bin) *.s a.out test.bin *.o

test.bin: all_tests.c lexer_test.c parser_test.c $(module)
	$(CC) $(CFLAGS) $^ -lcgreen -o $@

test: test.bin
	./$<

run: $(bin)
	cat ./input.txt
	./$< < input.txt

debug: $(bin)
	cat ./input.txt
	@read
	gdb ./$< -x gdb.cmd
