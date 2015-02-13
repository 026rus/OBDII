CC=g++ -Wall
CLEAN=rm -f
# Name of the program
PM=obdrw
# Object files
OF=*.o
# Source files
SF= Obdrw.cpp main.cpp


$(PM):  $(OF)
	$(CC) -o $@ $^

$(OF): $(SF)
	$(CC) -c $^

clean:
	$(CLEAN) *.o
	$(CLEAN) $(PM)
