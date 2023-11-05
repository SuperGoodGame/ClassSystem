# src = pipe.c
# obj = pipe.o
opt = -g -c

all: 
%.o: %.c
	gcc $(opt) -std=c99 $< -o $@
%: %.o
	gcc $< -l pthread -o $@
clean:
	rm tpipe  pipe *.o