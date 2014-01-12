#ifndef Parser_H_
#define Parser_H_

#include <vector>
#include <string>

#include "Declarations.h"
#include "Lexeme.h"

using std::vector;
using std::string;


class Parser {
public:
    Parser(vector<Lexeme>& lexemes);
    
    program_ptr get_parsed_program() {
        return m_program;
    }
    
private:
    Parser(Parser const&);
    Parser& operator=(Parser const&);
    
    Lexeme const& current_lexeme();
    Lexeme const& next_lexeme();
    void next_line();
    bool match_current_lexeme(LexemeTypes type); 
    
    program_ptr parse_program();
    instruction_ptr parse_instruction();
    instruction_ptr parse_function_definition();
    instruction_ptr parse_assignment();
    instruction_ptr parse_constant();
    instruction_ptr parse_variable();
    instruction_ptr parse_function_call();
    instruction_ptr parse_expression();
    instruction_ptr parse_condition();
    instruction_ptr parse_read();
    instruction_ptr parse_term();
    instruction_ptr parse_value();
    instruction_ptr parse_print();
    instruction_ptr parse_if_block();
    instruction_ptr parse_while_block();
    instruction_ptr parse_return();
        
    size_t m_current_lexeme_index;
    const vector<Lexeme>& m_lexemes;
    program_ptr m_program;
};

#endif