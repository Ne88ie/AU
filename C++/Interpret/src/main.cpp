#include <iostream>
#include <vector>
#include <cstddef>

using std::string;
using std::cout;

#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"
#include "Lexeme.h"
#include "ErrorHandler.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Not specified source file.\n";
        return 1;
    }

    string const code_file_path = argv[1];
    ifstream input_stream(code_file_path);

    size_t file_length = 0;
    input_stream.seekg(0, input_stream.end);
    file_length = input_stream.tellg();
    input_stream.seekg(0, input_stream.beg);

    if (file_length == 0) return 0;

    Lexer lexer(code_file_path);
    
    if (!ErrorHandler::is_ok()) return 3;
    
    vector<Lexeme> lexemes = lexer.get_result();
    Parser parser(lexemes);
    
    if (!ErrorHandler::is_ok()) return 4;

    Evaluator eval(*(parser.get_parsed_program()));    
    eval.execute_program();
    
    if (!ErrorHandler::is_ok()) return 5;
    
    return 0;
}
