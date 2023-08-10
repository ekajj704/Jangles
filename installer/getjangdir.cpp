#include <stdio.h>
#include <windows.h>
#include <string>

int main(){
    char buff[512];
    GetModuleFileName(NULL, buff, 512);
    std::string path = buff;
    path = path.substr(0, path.length() - 25);
    printf("%s\n", path.c_str());

    return 0;
}