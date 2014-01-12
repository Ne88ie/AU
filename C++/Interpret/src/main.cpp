#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cstddef>

using std::ifstream;
using std::string;
using std::cout;
using std::shared_ptr;

#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"
#include "Lexeme.h"
#include "ErrorHandler.h"


int main(int argc, const char* argv[]) {
    if (argc < 2) {
        cout << "Not specified source file.\n";
        return 1;
    }
    
    string code_file_path = argv[1];
    shared_ptr<ifstream> input_stream(new ifstream(code_file_path));
    
    if (!*input_stream) {
        cout << "File " << code_file_path << " not found.\n";
        return 2;
    }

    size_t file_length = 0;
    input_stream->seekg(0, input_stream->end);
    file_length = input_stream->tellg();
    input_stream->seekg(0, input_stream->beg);

    if (file_length == 0) return 0;

    Lexer lexer(input_stream);
    lexer.tokenize();
    
    if (!ErrorHandler::is_ok()) return 3;
    
    vector<Lexeme> lexemes = lexer.get_result();
    Parser parser(lexemes);
    
    if (!ErrorHandler::is_ok()) return 4;

    Evaluator eval(*(parser.get_parsed_program()));    
    eval.execute_program();
    
    if (!ErrorHandler::is_ok()) return 5;
    
    return 0;
}
