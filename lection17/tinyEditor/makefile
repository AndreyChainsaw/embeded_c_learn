OBJ_PATH = ./object
SRC_PATH = ./src

compile_debug: $(OBJ_PATH)/tinyEditor.o $(OBJ_PATH)/curses_ui.o $(OBJ_PATH)/ioworker.o
	mkdir -p $(SRC_PATH)
	cp 1.txt $(SRC_PATH)/1.txt
	gcc $(OBJ_PATH)/tinyEditor.o $(OBJ_PATH)/curses_ui.o $(OBJ_PATH)/ioworker.o -o $(SRC_PATH)/tinyEditor.debug -l ncurses -g

$(OBJ_PATH)/tinyEditor.o: tinyEditor.c
	mkdir -p $(OBJ_PATH)
	gcc -c tinyEditor.c -o $(OBJ_PATH)/tinyEditor.o -g

$(OBJ_PATH)/curses_ui.o: curses_ui.c
	mkdir -p $(OBJ_PATH)
	gcc -c curses_ui.c -o $(OBJ_PATH)/curses_ui.o -g

$(OBJ_PATH)/ioworker.o: ioworker.c
	mkdir -p $(OBJ_PATH)
	gcc -c ioworker.c -o $(OBJ_PATH)/ioworker.o

clean:
	rm -rf $(OBJ_PATH) $(SRC_PATH)
