#include "coder.h"
#include "decoder.h"
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

int main(int argc, char  *argv[])
{	
	bool regim_c;
	string s_in;
	string s_out;
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
		coder(s_in, s_out);
	}
	else{
		decoder(s_in, s_out);
	}
	
	return 0;
}