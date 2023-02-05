CC = gcc
LIB_DIR = ./lib/ #where your libraylib.a file is
OBJ_DIR = ./object_files/
SRC_DIR = ./src/
INCL_DIR = ./include/

LINKERS =-L $(LIB_DIR) -lraylib -lopengl32 -lgdi32 -lwinmm
OUTPUT_FLAGS = -Wall -std=c99 -Wno-missing-braces
CFLAGS = -static -fdiagnostics-color=always

build_debug: $(OBJ_DIR)main.o $(OBJ_DIR)gridCalc.o $(OBJ_DIR)tilePlacement.o
	$(CC) $(CFLAGS) -o game_of_life $(OUTPUT_FLAGS) $(OBJ_DIR)main.o $(OBJ_DIR)gridCalc.o $(OBJ_DIR)tilePlacement.o $(LINKERS)

build_release: $(OBJ_DIR)main.o
	$(CC) $(CFLAGS) -o game_of_life $(OUTPUT_FLAGS) $(OBJ_DIR)main.o $(OBJ_DIR)gridCalc.o $(LINKERS) -mwindows

#debugging on
$(OBJ_DIR)main.o: $(SRC_DIR)main.c
	$(CC) -g -c $(SRC_DIR)main.c
	mv main.o $(OBJ_DIR)

#debugging on
$(OBJ_DIR)gridCalc.o: $(SRC_DIR)gridCalc.c
	$(CC) -g -c $(SRC_DIR)gridCalc.c
	mv gridCalc.o $(OBJ_DIR)

#debugging on
$(OBJ_DIR)tilePlacement.o: $(SRC_DIR)tilePlacement.c
	$(CC) -g -c $(SRC_DIR)tilePlacement.c
	mv tilePlacement.o $(OBJ_DIR)

clean: 
	rm -f game_of_life $(OBJ_DIR)*.o

run: 
	./game_of_life
