exe: test.out clean

test.out: mypthread.o test.c
		gcc test.c mypthread.o -o test.out

mypthread.o: mypthread.h mypthread.c

clean:
		rm mypthread.o