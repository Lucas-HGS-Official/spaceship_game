GAME_NAME = ray_pong_clone

TARGET = bin/$(GAME_NAME)
WEB_TARGET = bin/$(GAME_NAME).html
SRC = $(wildcard code/src/*.c)
OBJ = $(patsubst code/src/%.c, code/obj/%.o, $(SRC))
WEB_LIBS = code/deps/static_libs/web/

CC = clang

LIB_SRC = $(wildcard code/libs/src/*.c)
LIB_OBJ = $(patsubst code/libs/src/%.c, code/libs/obj/%.o, $(LIB_SRC))

###################################
# Build Everything
###################################
default: from_scratch

from_scratch: clean_all build
###################################

###################################
# Build Game
###################################
build: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $? -std=c99 -Wall -Wextra -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

code/obj/%.o : code/src/%.c
	$(CC) -c $< -o $@ -Icode/include

###################################
# Tools
###################################
clean:
	rm -f $(TARGET)*

clean_all: clean
	rm -f code/obj/*.o

bear: clean_all
	bear -- make
###################################

###################################
# Build Web
###################################
update_emcc:
	~/emsdk/emsdk install latest

# activate_emcc:
# 	~/emsdk/emsdk activate latest
# 	source "/home/lhgs/emsdk/emsdk_env.sh"
# 	echo 'source "/home/lhgs/emsdk/emsdk_env.sh"' >> $HOME/.bash_profile

web_build: clean_all
	emcc -o $(WEB_TARGET) $(SRC) -Os -Wall ./$(WEB_LIBS)/libraylib.a -Icode/include -Icode/deps/include -I/home/lhgs/raylib/src -L$(WEB_LIBS) -s USE_GLFW=3 -s MAX_WEBGL_VERSION=2 -s ASYNCIFY -s MODULARIZE=1 --preload-file resources/sounds --preload-file resources/fonts -DPLATFORM_WEB
# -s EXPORT_NAME="game" -s TOTAL_MEMORY=67108864 -s ASSERTIONS=1 --profiling --shell-file path-to/shell.html
###################################

###################################
# Run Game
###################################
run: from_scratch
	./$(TARGET)

web_run:
	emrun $(TARGET).html
###################################
