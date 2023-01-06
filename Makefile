.PHONY: clean

CFLAGS := $(CFLAGS) -std=c99 -D_XOPEN_SOURCE=600 -Wall -Wpedantic

all: sig.exe randf.exe

sig.exe: sig.c
	cc $(CFLAGS) -o $@ $<

randf.exe: randf.c
	cc $(CFLAGS) -o $@ $<

clean:
	rm -rf *.exe

