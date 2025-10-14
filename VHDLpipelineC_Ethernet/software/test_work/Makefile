
OBJECTS += ../eth_sw.o

INCLUDE += -I../
INCLUDE += -I$(shell dirname `which pipelinec`)/../include/
INCLUDE += -I../../pipelinec_output/

work_test: $(OBJECTS) work_test.o
	gcc -o $@ $^

.PHONY: clean
clean:
	rm -rf work_test
	rm -rf *.o

%.o: %.c
	$(CC) -c $^ $(INCLUDE) -o $@
