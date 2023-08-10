#include "./filewriter.hpp"
#include "../main.hpp"

int writecfile(std::string name, std::string cbody){
    std::string filename = name + ".cpp";
    FILE* fptr = fopen(filename.c_str(), "w+");
    fprintf(fptr, "#include \"%s.hpp\"\n%s\n", name.c_str(), cbody.c_str());
    fclose(fptr);
    return 0;
}

int writehfile(std::string name, std::string hbody){
    std::string filename = name + ".hpp";
    FILE* fptr = fopen(filename.c_str(), "w+");
    
    std::string SDLimport = "";
    if(getflag(SDL_FLAG)){
        SDLimport = "#include <SDL2/SDL.h>";
        
    }
    
    fprintf(fptr, "#ifndef %s_H_\n#define %s_H_\n#include <janglib.hpp>\n%s\n%s#endif", name.c_str(), name.c_str(), SDLimport.c_str(),  hbody.c_str());
    fclose(fptr);
    return 0;
}

int writefiles(std::string name, std::string cbody, std::string hbody){
    writecfile(name, cbody);
    writehfile(name, hbody);
    return 0;
}