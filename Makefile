SB	:= #scan-build
CC	:= $(SB) clang
CFLAGS	:= -Wall -g3

module	:= context.c lexer.c parser.c token.c node.c ir.c
headers := context.h lexer.h parser.h token.h node.h ir.h
src	:= main.c $(module)
obj	:= $(src:.c=.o)
bin	:= uopcc

$(bin): $(obj) $(headers)
	$(CC) -o $@ $(obj)

clean:
	rm -rf $(obj) $(bin) *.s a.out test.bin *.o

test.bin: test.c $(module) lexer_test.c parser_test.c
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
