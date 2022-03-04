%.o : %.c *.h
	gcc -c $< -std=c99 -g3
app.exe: main.o list_queue.o array_queue.o structures.o
	gcc -o app.exe main.o list_queue.o array_queue.o structures.o -lm
clean:
	rm *.o *.exe
