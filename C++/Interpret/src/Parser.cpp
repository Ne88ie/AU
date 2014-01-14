#include <cstdlib>
#include <set>

#include "Parser.h"
#include "Exception.h"
#include "Instruction.h"


Parser::Parser(vector<Lexeme> const& lexemes):
    m_current_lexeme_index(0),
    m_lexemes(lexemes) {
        m_program = parse_program();
    }

Lexeme const& Parser::current_lexeme() {
    return m_lexemes[m_current_lexeme_index];
}

bool Parser::match_current_lexeme(Lexeme_Types type) {
    return current_lexeme().type == type;
}

void Parser::next_line() {
    if (match_current_lexeme(kEndofLine))
        next_lexeme();
    else
        throw Syntax_error(current_lexeme().line);
}

Lexeme const& Parser::next_lexeme() {
    if (m_current_lexeme_index + 1 < m_lexemes.size())
        return m_lexemes[++m_current_lexeme_index];

    return current_lexeme();
}

program_ptr Parser::parse_program() {
    size_t start_line = current_lexeme().line;
    instructions program_body;
    instructions functions;
    instruction_ptr instruction = instruction_ptr();
        
    if (match_current_lexeme(kEndofLine))
        next_line();

    while (!match_current_lexeme(kEndofFile)) {
        instruction = instruction_ptr();
        instruction = parse_function_definition();
        
        if (instruction) {
            functions.push_back(instruction);
            continue;
        }
        
        instruction = parse_instruction();
        
        if (!instruction) {
            throw Syntax_error(current_lexeme().line);
            return program_ptr();
        }
        
        program_body.push_back(instruction);
    }
    
    return program_ptr(new Program(start_line, program_body, functions));
}

instruction_ptr Parser::parse_instruction() {
    size_t m_buf_lexeme_index = m_current_lexeme_index;
    instruction_ptr result = parse_assignment();
    
    if (!result) {
        m_current_lexeme_index = m_buf_lexeme_index;
        result = parse_expression();
    }
    if (!result)
        result = parse_if_block();
    
    if (!result)
        result = parse_while_block();
    
    if (!result)
        result = parse_return();
    
    if (!result)
        result = parse_print();
    
    if (!result)
        result = parse_read();
    
    if (result)
        next_line();
    
    return result;
}

instruction_ptr Parser::parse_print() {
    size_t start_line = current_lexeme().line; 
    if (!match_current_lexeme(kPrintKeyword))
        return instruction_ptr();  

    next_lexeme();   
    instruction_ptr expression = parse_expression();
    if (!expression) {
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    } 
    return instruction_ptr(new Print(start_line, expression));
}

instruction_ptr Parser::parse_read() {
    size_t start_line = current_lexeme().line;
    if (!match_current_lexeme(kReadKeyword))
        return instruction_ptr();

    next_lexeme();
    if (!match_current_lexeme(kId)) {
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    string name = current_lexeme().value;
    next_lexeme();
    return instruction_ptr(new Read(start_line, name));
}

instruction_ptr Parser::parse_assignment() {
    size_t start_line = current_lexeme().line; 
    if (!match_current_lexeme(kId))
        return instruction_ptr();

    string name = current_lexeme().value;
    next_lexeme();
    if (!match_current_lexeme(kAssignment))
        return instruction_ptr();

    next_lexeme();
    instruction_ptr expression = parse_expression();
    if (!expression) {
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    return instruction_ptr(new Assignment(start_line, name, expression));
}

instruction_ptr Parser::parse_return() {
    size_t start_line = current_lexeme().line; 
    if (!match_current_lexeme(kReturnKeyword))
        return instruction_ptr();

    next_lexeme();
    instruction_ptr expression = parse_expression();
    if (!expression) {
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    return instruction_ptr(new Return(start_line, expression));
}

instruction_ptr Parser::parse_if_block() {
    size_t start_line = current_lexeme().line;
    if (!match_current_lexeme(kIfKeyword))
        return instruction_ptr();

    next_lexeme();
    instruction_ptr condition = parse_condition();
    if(!condition) {
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    if(!match_current_lexeme(kColon)) {
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    next_lexeme();
    next_line();
    instructions block;
    while (!match_current_lexeme(kEndKeyword)){
        instruction_ptr instruction = parse_instruction();
        if (!instruction){
            throw Syntax_error(current_lexeme().line);
            return instruction_ptr();
        }      
        block.push_back(instruction);
    }
    next_lexeme();
    return instruction_ptr(new If_Block(start_line, block, condition));
}

instruction_ptr Parser::parse_while_block(){
    size_t start_line = current_lexeme().line;
    if (!match_current_lexeme(kWhileKeyword))
        return instruction_ptr();

    next_lexeme();
    instruction_ptr condition = parse_condition();
    if(!condition){
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    if(!match_current_lexeme(kColon)){
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    next_lexeme();
    next_line();
    instructions block;
    while (!match_current_lexeme(kEndKeyword)) {
        instruction_ptr instruction = parse_instruction();
        if (!instruction) {
            throw Syntax_error(current_lexeme().line);
            return instruction_ptr();
        }
        block.push_back(instruction);
    }
    next_lexeme();
    return instruction_ptr(new While_Block(start_line, block, condition));
}

instruction_ptr Parser::parse_function_definition() {
    size_t start_line = current_lexeme().line;
    if (!match_current_lexeme(kDefKeyword))
        return instruction_ptr();

    next_lexeme();
    if (!match_current_lexeme(kId)) {
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    string name = current_lexeme().value;
    next_lexeme();
    if (!match_current_lexeme(kLeftBracket)) {
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    next_lexeme();
    vector<string> parameters;

    while(!match_current_lexeme(kRightBracket)) {
        if (!match_current_lexeme(kId)) {
            throw Syntax_error(current_lexeme().line);
            return instruction_ptr();
        }
        parameters.push_back(current_lexeme().value);
        next_lexeme();
        if (!match_current_lexeme(kComma)) continue;
        next_lexeme();
    }
    next_lexeme();
    if (!match_current_lexeme(kColon)) {
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    next_lexeme();
    next_line();
    instructions function_body;
    while (!match_current_lexeme(kEndKeyword)) {
        instruction_ptr instruction = parse_instruction();
        if (!instruction) {
            throw Syntax_error(current_lexeme().line);
            return instruction_ptr();
        }
        function_body.push_back(instruction);
    }
    next_lexeme();
    next_line();
    return instruction_ptr(new Function(start_line, function_body, name, parameters));
}

instruction_ptr Parser::parse_constant(){
    size_t start_line = current_lexeme().line;
    if (!match_current_lexeme(kNumber))
        return instruction_ptr();

    string value = current_lexeme().value;
    next_lexeme();
    return instruction_ptr(new Constant(start_line, atoi(value.c_str())));
}

instruction_ptr Parser::parse_expression() {
    size_t start_line = current_lexeme().line;
    instruction_ptr left = parse_term();
    if (!left) return instruction_ptr();

    while (current_lexeme().type == kAddition || current_lexeme().type == kSubtraction) {
        Lexeme lexeme = current_lexeme();
        next_lexeme();
        instruction_ptr right = parse_term();
        if (!right) {
            throw Syntax_error(current_lexeme().line);
            return instruction_ptr();
        }
        switch (lexeme.type) {
            case kAddition:
                left = instruction_ptr(new Arithmetic_Operation(start_line, Arithmetic_Operation::kAdd, left, right));
                break;
            case kSubtraction:
                left = instruction_ptr(new Arithmetic_Operation(start_line, Arithmetic_Operation::kSub, left, right));
                break;
            default:
                throw Syntax_error(current_lexeme().line);
                return instruction_ptr();
        }
    }
    return left;
}

instruction_ptr Parser::parse_function_call() {
    size_t start_line = current_lexeme().line;
    if (!match_current_lexeme(kId))
        return instruction_ptr();

    string name = current_lexeme().value;
    next_lexeme();
    if (!match_current_lexeme(kLeftBracket))
        return instruction_ptr(new Variable(start_line, name));

    next_lexeme();
    instructions parameters;
    if (!match_current_lexeme(kRightBracket)) {
        while (true) {
            instruction_ptr parameter = parse_expression();
            if (!parameter) {
                throw Syntax_error(current_lexeme().line);
                return instruction_ptr(); 
            }
            parameters.push_back(parameter);
            if (!match_current_lexeme(kComma)) break;
            else next_lexeme();
        }
        if (!match_current_lexeme(kRightBracket)) {
            throw Syntax_error(current_lexeme().line);
            return instruction_ptr();
        }
    }
    next_lexeme();
    return instruction_ptr(new Function_Call(start_line, name, parameters));
}

instruction_ptr Parser::parse_term() {
    size_t start_line = current_lexeme().line;
    instruction_ptr left = parse_value();
    if (!left) return instruction_ptr();

    while (current_lexeme().type == kMultiplication || current_lexeme().type == kDivision) {
        Lexeme lexeme = current_lexeme();
        next_lexeme();
        instruction_ptr right = parse_value();
        if (!right) {
            throw Syntax_error(current_lexeme().line);
            return instruction_ptr();
        }
        switch (lexeme.type) {
            case kMultiplication:
                left = instruction_ptr(new Arithmetic_Operation(start_line, Arithmetic_Operation::kMul, left, right));
                break;
            case kDivision:
                left = instruction_ptr(new Arithmetic_Operation(start_line, Arithmetic_Operation::kDiv, left, right));
                break;
            default:
                throw Syntax_error(current_lexeme().line);
                return instruction_ptr();
        }
    }
    return left;
}

instruction_ptr Parser::parse_value() {
    size_t start_line = current_lexeme().line;
    if (match_current_lexeme(kSubtraction)) {
        next_lexeme();
        instruction_ptr value = parse_value();
        if (!value) {
            throw Syntax_error(current_lexeme().line);
            return instruction_ptr();
        }
        return instruction_ptr(new Arithmetic_Operation(start_line,
                               Arithmetic_Operation::kSub,
                               instruction_ptr(new Constant(start_line, 0)),
                               value));
    }   
    if (match_current_lexeme(kLeftBracket)) {
        next_lexeme();
        instruction_ptr expression = parse_expression(); 
        if (!match_current_lexeme(kRightBracket)) {
            throw Syntax_error(current_lexeme().line);
            return instruction_ptr();
        }
        next_lexeme();
        return expression;
    }
    instruction_ptr value = parse_constant();
    if (!value) value = parse_function_call();
    return value;
}

instruction_ptr Parser::parse_condition() {
    size_t start_line = current_lexeme().line;
    instruction_ptr left = parse_expression();
    if (!left) {
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    Lexeme_Types logic_Operations[] = {kEqualOperation,
                                     kNotEqualOperation,
                                     kLessOperation,
                                     kGreaterOperation,
                                     kLessEqualOperation,
                                     kGreaterEqualOperation};
    std::set<Lexeme_Types> set_Logic_Operation (logic_Operations, logic_Operations + 6);
    if (set_Logic_Operation.count(current_lexeme().type) == 0) {
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    Lexeme lexeme = current_lexeme();
    next_lexeme();
    instruction_ptr right = parse_expression();
    if (!right) {
        throw Syntax_error(current_lexeme().line);
        return instruction_ptr();
    }
    switch (lexeme.type) {
        case kEqualOperation:
            return instruction_ptr(new Conditional(start_line, Conditional::kEqual, left, right));
        case kNotEqualOperation:
            return instruction_ptr(new Conditional(start_line, Conditional::kNotEqual, left, right));
        case kLessOperation:
            return instruction_ptr(new Conditional(start_line, Conditional::kLess, left, right));
        case kGreaterOperation:
            return instruction_ptr(new Conditional(start_line, Conditional::kGreater, left, right));
        case kLessEqualOperation:
            return instruction_ptr(new Conditional(start_line, Conditional::kLessEqual, left, right));
        case kGreaterEqualOperation:
            return instruction_ptr(new Conditional(start_line, Conditional::kGreaterEqual, left, right));
        default:
            throw Syntax_error(current_lexeme().line);
            return instruction_ptr();
    }
}
