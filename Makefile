yash: main.o
	gcc -g main.o -lreadline -o yash

main.o: main.c
	gcc -g -c main.c

clean:
	rm *.o yash
