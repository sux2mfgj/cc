CC	:= cc
CFLAGS	:= -Wall -ggdb3

src	:= main.c
obj	:= $(src:.c=.o)
bin	:= uopcc

$(bin): $(obj)
	echo $^
	$(CC) -o $@ $^

clean:
	rm -rf $(obj) $(bin) *.s a.out

include  test.mk
