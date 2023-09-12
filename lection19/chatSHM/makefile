OBJ=./obj/
BIN=./bin/

compileAll: compileServer compileClient

compileServer: $(OBJ)/server.o $(OBJ)/chatCommon.o
	mkdir -p $(BIN)
	gcc $(OBJ)/server.o $(OBJ)/chatCommon.o -o $(BIN)/server -g

compileClient: $(OBJ)/client.o $(OBJ)/chatCommon.o
	mkdir -p $(BIN)/
	gcc $(OBJ)/client.o $(OBJ)/chatCommon.o -o $(BIN)/client -g

$(OBJ)/client.o: client.c
	mkdir -p ./$(OBJ)/
	gcc -c client.c -o ./$(OBJ)/client.o -g

$(OBJ)/chatCommon.o: chatCommon.c
	mkdir -p ./$(OBJ)/
	gcc -c chatCommon.c -o ./$(OBJ)/chatCommon.o -g

$(OBJ)/server.o: server.c
	mkdir -p ./$(OBJ)/
	gcc -c server.c -o ./$(OBJ)/server.o -g

clean:
	rm -rf $(OBJ) $(BIN)