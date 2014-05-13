all: out

out:	user.o queue.o
		gcc -o out  user.o queue.o

mmap_user.o:	user.c
		gcc -c user.c

queue.o:	../queue.c
		gcc -c ../queue.c
