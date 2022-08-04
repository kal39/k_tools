log: log_test
	./test/log_test

log_test: out
	gcc test/log_test.c -o test/log_test

out:
	mkdir -p test/out/

clean:
	rm -rf test/out/