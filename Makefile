.PHONY: clean

sig.exe: sig.c
	cc -std=c89 -D_XOPEN_SOURCE=600 -Wall -Wpedantic -o $@ $<

clean:
	rm -rf *.exe

