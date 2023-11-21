objects = ./src/lexer/lexer.o ./src/classes/function.o ./src/parser/parser.o ./src/debug/debugprint.o ./src/libs/librarian.o ./src/libs/bookInjector.o ./src/exec/exec.o ./src/main.o

build: $(objects) ./src/libs/librarian.hpp
	
	g++ $(objects) -o Jangles

clean:
	rm $(objects)