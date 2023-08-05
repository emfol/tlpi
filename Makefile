.PHONY: test clean

CFLAGS := $(CFLAGS) -std=c99 -D_XOPEN_SOURCE=600 -Wall -Wpedantic

all: sig.exe randf.exe fib.exe

sig.exe: sig.c
	cc $(CFLAGS) -o $@ $<

randf.exe: randf.c
	cc $(CFLAGS) -o $@ $<

fib.exe: fib.c
	cc -ansi -pedantic -Wall -o $@ $<

test: fib.exe
	./test_fib.sh

clean:
	rm -rf *.exe
