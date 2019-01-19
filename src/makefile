CC=gcc
CFLAGS=-I. -Wall -Werror -fpic -std=c11
#CFLAGS=-I. -Wall -Werror -fpic -D DEBUG -D TEST -g 
DEPS=sim800l.h
SOURCE=sim800l.c
OBJ=sim800l.o 

LDFLAGS = -shared
LINT=cppcheck --language=c --enable=warning,style,unusedFunction,information --template=gcc 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

libsim800l.so: $(OBJ)
	$(CC) -o $@ $^  $(LDFLAGS)
	
.PHONY: clean
clean:
	rm -f *.o  
	rm -f *.so

.PHONY: indent
indent:
	indent -kr -bl -bli0 -ci2 -cli2 -i2 -l80 -nut -nce *.c 

.PHONY: indent1
indent1:
	indent -nbad -bap -nbc -bbo -bl -bli2 -bls -ncdb -nce -cp1 -cs -di2 -ndj -nfc1 -nfca -hnl -i2 -ip5 -lp -pcs -psl -nsc -nsob *.c 
	
.PHONY: install
install:
	sudo cp -f -v libsim800l.so /usr/local/bin
	sudo chmod 755 /usr/local/bin/libsim800l.so

.PHONY: uninstall
uninstall:
		rm -f /usr/local/bin/libsim800l.so

.PHONY: lint
lint :
	$(LINT) $(SOURCE) 


