CC=gcc
CFLAGS=-I. -I../src -DTRACE -g  -std=c11
#CFLAGS=-I. -I../src -D DEBUG -DTEST -g 
#CFLAGS=-I. -I../src 
SOURCE=test_gsm.c log.c
export ROOT_DIR=${PWD}
OBJ= test_gsm.o log.o
LDFLAGS = -Wl,-rpath=${PWD}/../src  -L${PWD}/../src -lwiringPi -lquickmail  -lsim800l
LINT=cppcheck --language=c --enable=warning,style,unusedFunction,information --template=gcc 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

    
test_gsm_tests: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	
.PHONY: clean
clean:
	rm -f *.o  
	rm -f test_gsm_tests

.PHONY: indent
indent:
	indent -kr -bl -bli0 -ci2 -cli2 -i2 -l80 -nut -nce *.c 
	indent -kr -bl -bli0 -ci2 -cli2 -i2 -l80 -nut -nce *.h 
		
.PHONY: lint
lint :
	$(LINT) $(SOURCE) 

.PHONY: test
test: test_gsm_tests
	unset LD_LIBRARY_PATH
	./test_gsm_tests
