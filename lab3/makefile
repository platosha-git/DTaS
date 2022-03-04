%.o : %.c *.h
	gcc -c $< -std=c99 -g3
app.exe: main.o mem_free.o input.o changes.o output.o time.o
	gcc -o app.exe main.o mem_free.o input.o changes.o output.o time.o
clean:
	rm *.o *.exe
