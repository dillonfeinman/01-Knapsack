CFLAGS = -Wall -g -Wextra -DDebug
P = BestFirstSearch

all: $(P)

$(P): $(P).cpp
	g++ -c $(P).cpp
	g++ -g $(P).o -o $(P)


run: all
	./$(P) input.txt output.txt


clean:
	rm -rf *.o $(P) $(O) $(N) output*

checkmem: all
	valgrind -v --leak-check=full --track-origins=yes ./$(P)
