
tests: test_0 test_1

test_0: $(bin)
	echo 0 | ./$(bin) > test.s
	$(CC) test.s
	 ./a.out

test_1: $(bin)
	echo 12 + 2 | ./$(bin) > test.s
	$(CC) test.s
	 ./a.out
