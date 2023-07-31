build:
	g++ -Wall -c ./src/main.cpp ./src/lexer/lexer.cpp ./src/parser/parser.cpp ./src/parser/Node.cpp ./src/filewriter/filewriter.cpp
	g++ ./src/main.cpp ./src/lexer/lexer.cpp ./src/parser/parser.cpp ./src/parser/Node.cpp ./src/filewriter/filewriter.cpp -o Jangles.exe

clean:
	del lexer.o main.o parser.o Node.o filewriter.o
