CC=g++ -Wall -std=c++11 -pthread
CLEAN=rm -f
# Name of the program
PM=server1
# Object files
OF=*.o
# Source files
SF= MyServer.cpp main.cpp


$(PM):  $(OF)
	$(CC) -o $@ $^

$(OF): $(SF)
	$(CC) -c $^

clean:
	$(CLEAN) *.o
	$(CLEAN) $(PM)
