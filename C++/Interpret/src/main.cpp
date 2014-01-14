#include <iostream>
#include <vector>
#include <cstddef>

using std::cerr;
using std::string;
using std::cout;

#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"
#include "Lexeme.h"
#include "Exception.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Not specified source file.\n";
        return 1;
    }

    string const code_file_path = argv[1];

    try
        {
            Lexer lexer(code_file_path); 
            Parser parser(lexer.get_result());
            Evaluator eval(parser.get_parsed_program());    
            eval.execute_program();
        }
        catch (Exception const &error)
        {
            cerr << "line " <<  error.get_line() <<": "<< error.what() << ".\n";
            return 2;
        }

    return 0;
}
