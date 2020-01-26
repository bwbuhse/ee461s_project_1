yash: main.o
	gcc main.o -lreadline -o yash

main.o: main.c
	gcc -c main.c

clean:
	rm *.o yash