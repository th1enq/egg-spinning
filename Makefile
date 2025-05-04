.PHONY: clean run
.SILENT:

egg.o: egg.c
	gcc -o $@ $< -lm

run: egg.o
	./$<

clean:
	rm -rf egg.o

