TARGET = bin/game
SRC = $(wildcard code/src/*.c)
OBJ = $(patsubst code/src/%.c, code/obj/%.o, $(SRC))
LIB_SRC = $(wildcard code/libs/src/*.c)
LIB_OBJ = $(patsubst code/libs/src/%.c, code/libs/obj/%.o, $(LIB_SRC))

###################################
# Build Everything
###################################
default: from_scratch

from_scratch: clean build
###################################

###################################
# Build Game
###################################
build: $(TARGET)

$(TARGET): $(OBJ)
	gcc -o $@ $? -Wall -std=c99 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

code/obj/%.o : code/src/%.c
	gcc -c $< -o $@ -Icode/include

clean:
	rm -f code/obj/*.o
	rm -f $(TARGET)
###################################

###################################
# Run Game
###################################
run:
	./$(TARGET)