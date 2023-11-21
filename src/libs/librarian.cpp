#include "librarian.hpp"

vector<string> labels;
vector<Book*> books;


int InjectBook(Book *newBook){
    books.push_back(newBook);
    labels.push_back(newBook->Name());
    return 0;
}

vector<Token> ImportBook(string bookName){
    bookName += ".hpp";
    FILE* fptr = fopen(bookName.c_str(), "r");

    if(fptr == NULL){
        string error = "Cannot import book " + bookName + "\n";
        perror(error.c_str());
    }

    fclose(fptr);


    return vector<Token>();
}

vector<Token> RunBookFunc(vector<Token>* LHS, vector<Token>* RHS){
    auto book = LHS->front();
    auto func = RHS->front();
    RHS->erase(RHS->begin());
    auto args = RHS;


    int index = -1;

    for(size_t i = 0; i < labels.size(); i++){
        if(labels.at(i) == book.val){
            index = i;
            break;
        }
    }
    
    if(index == -1){
        string error = "No namespace matching the name " + book.val;
        perror(error.c_str());
        exit(-1);
    }

    if(!(books.at(index)->HasFunction(func.val))){
        string error = "No function matching the name <" + func.val + "> in namespace <" + book.val + ">";
        perror(error.c_str());
        exit(-1);
    }

    return books.at(index)->RunFunction(func.val, args);
}