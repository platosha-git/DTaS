%.o : %.c *.h
	gcc -c $< -std=c99 -g3
app.exe: main.o analyse.o hash.o tree.o
	gcc -o app.exe main.o analyse.o hash.o tree.o -lm
clean:
	rm *.o *.exe
