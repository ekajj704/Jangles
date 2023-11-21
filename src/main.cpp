#include "./headers/main.hpp"
#include "./headers/parser.hpp"
#include "./libs/bookInjector.hpp"

int main(int argc, char** argv){

    InjectBooks();

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

    exit(0);
}