yash: main.o
	gcc -std=c11 main.o -lreadline -o yash

main.o: main.c
	gcc -std=c11 -c main.c

clean:
	rm *.o yash
