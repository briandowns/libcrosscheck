cc = cc

NAME = libcrosscheck

UNAME_S = $(shell uname -s)

CFLAGS  = -std=c17 -O3 -fPIC -Wall -Wextra
LDFLAGS =

# respect traditional UNIX paths
INCDIR  = /usr/local/include
LIBDIR  = /usr/local/lib

ifeq ($(UNAME_S),Darwin)
$(NAME).dylib: clean
	$(CC) -dynamiclib -o $@ crosscheck.c $(CFLAGS) $(LDFLAGS)
else
$(NAME).so: clean
	$(CC) -shared -o $@ crosscheck.c $(CFLAGS) $(LDFLAGS)
endif

.PHONY: tests
tests: example
	./example
	make clean

.PHONY: valgrind
valgrind: tests
	valgrind --leak-check=full ./tests/tests 2>&1 | awk -F':' '/definitely lost:/ {print $2}'

.PHONY: install
install: 
	cp crosscheck.h $(INCDIR)
ifeq ($(UNAME_S),Darwin)
	cp $(NAME).dylib $(LIBDIR)
else
	cp $(NAME).so $(LIBDIR)
endif

uninstall:
	rm -f $(INCDIR)/crosscheck.h
ifeq ($(UNAME_S),Darwin)
	rm -f $(INCDIR)/$(NAME).dylib
else
	rm -f $(INCDIR)/$(NAME).so
endif

.PHONY: clean
clean:
	rm -f $(NAME).dylib
	rm -f $(NAME).so
	rm -f example
	rm -f tests/tests
	rm -f test

# compiling with debug symbols for easier testing the example code.
.PHONY: example
example: clean
	$(CC) -g -o $@ crosscheck.c example.c $(CFLAGS) $(LDFLAGS)
