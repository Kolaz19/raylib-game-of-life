CC = gcc
LIB_DIR = ./lib/ #where your libraylib.a file is
OBJ_DIR = ./object_files/
SRC_DIR = ./src/
INCL_DIR = ./include/

LINKERS =-L $(LIB_DIR) -lraylib -lopengl32 -lgdi32 -lwinmm
OUTPUT_FLAGS = -Wall -std=c99 -Wno-missing-braces
CFLAGS = -static -fdiagnostics-color=always

build_debug: $(OBJ_DIR)main.o 
	$(CC) $(CFLAGS) -g -o  game_of_life $(OUTPUT_FLAGS) $(OBJ_DIR)main.o $(LINKERS)

build_release: $(OBJ_DIR)main.o
	$(CC) $(CFLAGS) -o  game_of_life $(OUTPUT_FLAGS) $(OBJ_DIR)main.o $(LINKERS) -mwindows

$(OBJ_DIR)main.o: $(SRC_DIR)main.c
	$(CC) -c $(SRC_DIR)main.c
	mv main.o $(OBJ_DIR)

clean: 
	rm -f game_of_life $(OBJ_DIR)*.o

run: 
	./game_of_life
