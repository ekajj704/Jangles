#include "./filewriter.hpp"

int writecfile(std::string name, std::string cbody){
    std::string filename = name + ".cpp";
    FILE* fptr = fopen(filename.c_str(), "w+");
    fprintf(fptr, "#include \"%s.hpp\"\nusing namespace std;\n%s", name.c_str(), cbody.c_str());
    fclose(fptr);
    return 0;
}

int writehfile(std::string name, std::string hbody){
    std::string filename = name + ".hpp";
    FILE* fptr = fopen(filename.c_str(), "w+");
    fprintf(fptr, "#ifndef %s_H_\n#define %s_H_\n#include <iostream>\n#include <string>\n#include <vector>\n%s#endif", name.c_str(), name.c_str(), hbody.c_str());
    fclose(fptr);
    return 0;
}

int writefiles(std::string name, std::string cbody, std::string hbody){
    writecfile(name, cbody);
    writehfile(name, hbody);
    return 0;
}