#!/bin/bash
HEADER=./src/libs/all_books.hpp
INJECTOR=./src/libs/bookInjector.cpp

echo "#ifndef __ALL_BOOKS__" > $HEADER
echo "#define __ALL_BOOKS__" >> $HEADER

echo "#include \"all_books.hpp\"" > $INJECTOR
echo "#include \"librarian.hpp\"" >> $INJECTOR
echo "void InjectBooks(){" >> $INJECTOR

COUNTER=0

for file in src/libs/bookshelf/*.hpp
do

STRING=$(echo "$file"|cut -d'/' -f3-)
echo "#include \"./$STRING\"">> $HEADER

STRING=$(echo "$STRING"|cut -d'/' -f2-);
STRING=$(echo "$STRING"|cut -d'.' -f1);
echo "Book* buff$COUNTER = new $STRING();" >> $INJECTOR
echo "InjectBook(buff$COUNTER);" >> $INJECTOR

let COUNTER++
done
echo "#endif" >> $HEADER
echo "}" >> $INJECTOR