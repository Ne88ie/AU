#include "encode.h"
#include "decode.h"
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char const *argv[])
{
    bool regim_c;
    std::string s_in;
    std::string s_out;
    for (int i = 1; i < argc; ++i){
        if (strcmp(argv[i],"-c") == 0){
            regim_c = 1;
        }
        if (strcmp(argv[i], "-d") == 0){
            regim_c = 0;
        }
        if (strcmp(argv[i], "-i") == 0){
            s_in = argv[i+1];
            ++i;
        }
        if (strcmp(argv[i], "-o") == 0){
            s_out = argv[i+1];
            ++i;
        }
    }
    if (regim_c){
        encode(s_in, s_out);
    }
    else{
        decode(s_in, s_out);
    }

    return 0;
}