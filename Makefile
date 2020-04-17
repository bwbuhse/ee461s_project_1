yash: yash.o
	gcc -g yash.o -L/usr/include/readline -lreadline -o yash

yash.o: yash.c
	gcc -g -c yash.c

clean:
	rm *.o yash
