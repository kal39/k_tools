
all: log result

log: clean ./out/log_test
	./out/log_test

result: clean ./out/result_test
	./out/result_test

./out/log_test: out
	gcc -std=c11 -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wno-unused-parameter -Wno-cast-qual -O3 test/log_test.c -o out/log_test

./out/result_test: out
	gcc -std=c11 -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wno-unused-parameter -Wno-cast-qual -O3 test/result_test.c -o out/result_test

out:
	mkdir -p out/

clean:
	rm -rf out/