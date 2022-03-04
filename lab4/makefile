%.o : %.c *.h
	gcc -c $< -std=c99 -g3
app.exe: main.o array_stack.o list_stack.o parentheses.o analysis.o
	gcc -o app.exe main.o array_stack.o list_stack.o parentheses.o analysis.o
clean:
	rm *.o *.exe
