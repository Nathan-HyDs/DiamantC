OBJ := diamond.o game.o
EXENAME := diamond

CC := gcc
CFLAGS := -Wall -O3 -g -std=gnu99

all : exe

exe : $(EXENAME)

$(EXENAME) : $(OBJ)
	gcc $^ -g -o $@

clean:
	rm -f $(OBJ)
	rm -f *~
	rm -f $(EXENAME)
