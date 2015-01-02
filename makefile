CC = g++

minesweeper: 
	$(CC) main.cpp -o minesweeper  

run: 
	./minesweeper

clean:
	rm -f minesweeper main.o
