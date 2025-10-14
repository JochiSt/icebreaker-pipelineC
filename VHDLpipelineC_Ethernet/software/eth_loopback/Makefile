
OBJECTS += ../eth_sw.o

loopback_test: $(OBJECTS) loopback_test.o
	gcc -o $@ $^

.PHONY: clean
clean:
	rm -rf loopback_test
	rm -rf *.o

%.o: %.c
	$(CC) -c $^ -I../ -o $@
