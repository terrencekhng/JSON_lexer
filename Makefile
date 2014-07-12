OBJ = ./test/test_token.o ./src/token.o ./src/utils.o ./src/lexer.o

JSON_lexer: $(OBJ)
	mkdir ./build | gcc -o ./build/JSON_lexer $(OBJ)

BUILD_DIR: ./build

test_token.o: ./test/test_token.c ./src/token.h ./src/lexer.h ./src/utils.h
	cc -c ./test/test_token.c

token.o: ./src/token.c ./src/token.h
	cc -c ./src/token.c

utils.o: ./src/utils.c ./src/utils.h
	cc -c ./src/utils.c

lexer.o: ./src/lexer.c ./src/lexer.h ./src/token.h
	cc -c ./src/lexer.c
