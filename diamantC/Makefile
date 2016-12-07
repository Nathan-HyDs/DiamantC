OBJ := diamond.o game.o
EXENAME := diamond

CC := gcc
CFLAGS := -Wall -O3 -g

all : exe

exe : $(EXENAME)

$(EXENAME) : $(OBJ)
	gcc $^ -g -o $@

clean:
	rm -f $(OBJ)
	rm -f *~
	rm -f $(EXENAME)
