TARGET = bin/game
SRC = $(wildcard code/src/*.c)
OBJ = $(patsubst code/src/%.c, code/obj/%.o, $(SRC))

build: clean $(TARGET)

$(TARGET): $(OBJ)
	gcc -o $@ $? -Wall -std=c99 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

code/obj/%.o : code/src/%.c
	gcc -c $< -o $@ -Icode/include

clean:
	rm -f code/obj/*.o
	rm -f $(TARGET)

run:
	./$(TARGET)