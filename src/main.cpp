#include "./headers/main.hpp"
#include "./headers/parser.hpp"
#include "./libs/all_books.hpp"

int main(int argc, char** argv){

    if(argc < 2){
        fprintf(stderr, "ERROR: No file specified\n");
        exit(1);
    }

    FILE* fptr = fopen(argv[1], "r");

    if(fptr == NULL){
        fprintf(stderr, "ERROR: Cannot open file: %s\n", argv[1]);
        exit(1);
    }

    ParseFile(fptr);

    fcloseall();
    
    return 0;
}