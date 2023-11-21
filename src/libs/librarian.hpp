#ifndef LIBRARIAN_H_
#define LIBRARIAN_H_

#include "book.hpp"

int InjectBook(Book* newBook);
vector<Token> RunBookFunc(vector<Token>* LHS, vector<Token>* RHS);

#endif