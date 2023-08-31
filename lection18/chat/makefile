OBJ_PATH = ./obj
BIN_PATH = ./bin

compile_debug: $(OBJ_PATH)/listClients.o $(OBJ_PATH)/hash.o $(OBJ_PATH)/server.o $(OBJ_PATH)/client.o
	mkdir -p $(BIN_PATH)
	gcc $(OBJ_PATH)/listClients.o $(OBJ_PATH)/hash.o $(OBJ_PATH)/server.o -o $(BIN_PATH)/server -g
#	gcc $(OBJ_PATH)/hash.o $(OBJ_PATH)/client.o $(BIN_PATH)/client -g

compile_debug_client: $(OBJ_PATH)/hash.o $(OBJ_PATH)/client.o
	mkdir -p $(BIN_PATH)
	gcc $(OBJ_PATH)/client.o $(OBJ_PATH)/hash.o -o $(BIN_PATH)/client -lncurses

$(OBJ_PATH)/listClients.o: listClients.c
	mkdir -p $(OBJ_PATH)
	gcc -c listClients.c -o $(OBJ_PATH)/listClients.o -g

$(OBJ_PATH)/hash.o: hash.c
	mkdir -p $(OBJ_PATH)
	gcc -c hash.c -o $(OBJ_PATH)/hash.o -g

$(OBJ_PATH)/server.o: server.c
	mkdir -p $(OBJ_PATH)
	gcc -c server.c -o $(OBJ_PATH)/server.o -g

$(OBJ_PATH)/client.o: client.c
	mkdir -p $(OBJ_PATH)
	gcc -c client.c -o $(OBJ_PATH)/client.o

clean:
	rm -rf $(OBJ_PATH) $(BIN_PATH)
