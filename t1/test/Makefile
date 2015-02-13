CC=g++ -Wall
CLEAN=rm -f
# Name of the program
PM=test
# Object files
OF=*.o
# Source files
SF= main.cpp


$(PM):  $(OF)
	$(CC) -o $@ $^

$(OF): $(SF)
	$(CC) -c $^

clean:
	$(CLEAN) *.o
	$(CLEAN) $(PM)
