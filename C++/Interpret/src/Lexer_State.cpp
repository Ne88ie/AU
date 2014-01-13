#include "Lexer_State.h"
#include "Exception.h"


Symbol_Types Lexer_State::determine_symbol_type(char symbol) {
    if (isalpha(symbol)) return kLetter;  
    if (isdigit(symbol)) return kDigit;
    if (symbol == '\n') return kLinefeed;   
    if (isspace(symbol)) return kWhitespace;
    
    switch (symbol) {
        case '_': return kUnderscore;

        case '+':
        case '-':
        case '/':
        case '*':
        case '<':
        case '>':
        case '=':
        case '!': return kOperationSymbol;

        case ',':
        case '(':
        case ')':
        case ':': return kSeparator;

        default: return kUnknownType;
    }
}

const std::unordered_map<string, Lexeme_Types> map_Types_Alnumerics = {
    {"if", kIfKeyword},
    {"end", kEndKeyword},
    {"def", kDefKeyword},
    {"read", kReadKeyword},
    {"print", kPrintKeyword},
    {"while", kWhileKeyword},
    {"return", kReturnKeyword}
};

const std::unordered_map<char, Lexeme_Types> map_Types_Seps = {
    {',', kComma},
    {':', kColon},
    {'(', kLeftBracket},
    {')', kRightBracket},
};

const std::unordered_map<string, Lexeme_Types> map_Types_Operators = {
    {"=", kAssignment},
    {"<", kLessOperation},
    {">", kGreaterOperation},
    {"-", kSubtraction},
    {"+", kAddition},
    {"/", kDivision},
    {"*", kMultiplication},
    {"==", kEqualOperation},
    {"!=", kNotEqualOperation},
    {"<=", kLessEqualOperation},
    {">=", kGreaterEqualOperation}
};

Lexeme_Types handle_separator(char separator) {
    return map_Types_Seps.count(separator) != 0 ? map_Types_Seps.find(separator)->second : kUnknownLexeme;
}

Lexeme_Types handle_alphanumeric(string alphanumeric) {
    return map_Types_Alnumerics.count(alphanumeric) != 0 ? map_Types_Alnumerics.find(alphanumeric)->second : kId;
}

Lexeme_Types handle_operation(string operation) {
    return map_Types_Operators.count(operation) != 0 ? map_Types_Operators.find(operation)->second : kUnknownLexeme;
}


void Empty_State::next_state(Lexing_Automation& automation, char symbol) {
    switch (determine_symbol_type(symbol)) {
        case kLetter:
            automation.set_state(state_ptr(new Alphanumeric_State()));
            break;
            
        case kDigit:
            automation.set_state(state_ptr(new Numeric_State()));
            break;
            
        case kOperationSymbol:
            automation.set_state(state_ptr(new Operation_State()));
            break;
            
        case kSeparator:
            automation.set_result(Lexeme(handle_separator(automation.buffer().back()), "", automation.line()));
            automation.clear_buffer();
            break;
            
        case kLinefeed:
            automation.set_result(Lexeme(kEndofLine, "",automation.line()));
            automation.clear_buffer();
            break;
            
        case kWhitespace:
            automation.clear_buffer();
            break;
     
        default:
            throw Syntax_error(automation.line());
    }    
}

void Alphanumeric_State::next_state(Lexing_Automation &automation, char symbol) {
    switch (determine_symbol_type(symbol)) {
        case kUnderscore:
        case kLetter:
        case kDigit: break;
            
        case kOperationSymbol:
            automation.set_result(Lexeme(handle_alphanumeric(automation.get_buffer_prefix()), automation.get_buffer_prefix(), automation.line()));
            automation.set_buffer_to_last_char();
            automation.set_state(state_ptr(new Operation_State()));
            break;
            
        case kSeparator:
            automation.set_result(Lexeme(handle_alphanumeric(automation.get_buffer_prefix()), automation.get_buffer_prefix(), automation.line()));
            automation.set_result(Lexeme(handle_separator(automation.buffer().back()),  "", automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new Empty_State()));
            break;
        
        case kLinefeed:
            automation.set_result(Lexeme(handle_alphanumeric(automation.get_buffer_prefix()), automation.get_buffer_prefix(), automation.line()));
            automation.set_result(Lexeme(kEndofLine, "", automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new Empty_State()));
            break;
            
        case kWhitespace:
            automation.set_result(Lexeme(handle_alphanumeric(automation.get_buffer_prefix()), automation.get_buffer_prefix(), automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new Empty_State()));
            break;
                        
        default:
            throw Syntax_error(automation.line());
    }
}

void Numeric_State::next_state(Lexing_Automation &automation, char symbol) {
    switch (determine_symbol_type(symbol)) {
        case kDigit:
            break;
            
        case kOperationSymbol:
            automation.set_result(Lexeme(kNumber, automation.get_buffer_prefix(), automation.line()));
            automation.set_buffer_to_last_char();
            automation.set_state(state_ptr(new Operation_State()));
            break;
            
        case kSeparator:
            automation.set_result(Lexeme(kNumber, automation.get_buffer_prefix(), automation.line()));
            automation.set_result(Lexeme(handle_separator(automation.buffer().back()), "", automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new Empty_State()));
            break;
        
        case kLinefeed:
            automation.set_result(Lexeme(kNumber, automation.get_buffer_prefix(), automation.line()));
            automation.set_result(Lexeme(kEndofLine, "", automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new Empty_State()));
            break;
            
        case kWhitespace:
            automation.set_result(Lexeme(kNumber, automation.get_buffer_prefix(), automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new Empty_State()));
            break;
            
        default:
            throw Syntax_error(automation.line());
    }
}

void Operation_State::next_state(Lexing_Automation& automation, char symbol) {
    switch (determine_symbol_type(symbol)) {
        case kLetter:
            automation.set_result(Lexeme(handle_operation(automation.get_buffer_prefix()), "", automation.line()));
            automation.set_buffer_to_last_char();
            automation.set_state(state_ptr(new Alphanumeric_State()));
            break;

        case kDigit:
            automation.set_result(Lexeme(handle_operation(automation.get_buffer_prefix()), "", automation.line()));
            automation.set_buffer_to_last_char();
            automation.set_state(state_ptr(new Numeric_State()));
            break;
            
        case kOperationSymbol:
            if (map_Types_Operators.count(automation.buffer()) == 0) {
                automation.set_result(Lexeme(handle_operation(automation.get_buffer_prefix()), "", automation.line()));
                string lexeme_symbol(1, automation.buffer().back());
                automation.set_result(Lexeme(handle_operation(lexeme_symbol), "" , automation.line()));
                automation.clear_buffer();
                automation.set_state(state_ptr(new Empty_State()));
            } else {
                automation.set_result(Lexeme(handle_operation(automation.buffer()), "", automation.line()));
                automation.clear_buffer();
                automation.set_state(state_ptr(new Empty_State()));
            }
            break;
            
        case kSeparator:
            automation.set_result(Lexeme(handle_operation(automation.get_buffer_prefix()), "", automation.line()));
            automation.set_result(Lexeme(handle_separator(automation.buffer().back()), "", automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new Empty_State()));
            break;
            
        case kWhitespace:
            automation.set_result(Lexeme(handle_operation(automation.get_buffer_prefix()), "", automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new Empty_State()));
            break;
            
        default:
            throw Syntax_error(automation.line());
    }
}
