head = test1.h
srcs = test1.c
objs = test1.o
opts = -g -c
all: test
test: $(objs)	
	gcc $(objs) -o test1 
test.o: $(srcs) $(head)
	gcc $(opts) $(srcs) 
clean:
	rm test1 *.o