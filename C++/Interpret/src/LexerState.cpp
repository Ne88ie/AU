#include "LexerState.h"
#include "Exception.h"


SymbolTypes LexerState::determine_symbol_type(char symbol) {
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

const std::unordered_map<string, LexemeTypes> mapTypesAlnum = {
    {"if", kIfKeyword},
    {"end", kEndKeyword},
    {"def", kDefKeyword},
    {"read", kReadKeyword},
    {"print", kPrintKeyword},
    {"while", kWhileKeyword},
    {"return", kReturnKeyword}
};

const std::unordered_map<char, LexemeTypes> mapTypesSep = {
    {',', kComma},
    {':', kColon},
    {'(', kLeftBracket},
    {')', kRightBracket},
};

const std::unordered_map<string, LexemeTypes> mapTypesOp = {
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

LexemeTypes handle_separator(char separator) {
    return mapTypesSep.count(separator) != 0 ? mapTypesSep.find(separator)->second : kUnknownLexeme;
}

LexemeTypes handle_alphanumeric(string alphanumeric) {
    return mapTypesAlnum.count(alphanumeric) != 0 ? mapTypesAlnum.find(alphanumeric)->second : kId;
}

LexemeTypes handle_operation(string operation) {
    return mapTypesOp.count(operation) != 0 ? mapTypesOp.find(operation)->second : kUnknownLexeme;
}


void EmptyState::next_state(LexingAutomation& automation, char symbol) {
    switch (determine_symbol_type(symbol)) {
        case kLetter:
            automation.set_state(state_ptr(new AlphanumericState()));
            break;
            
        case kDigit:
            automation.set_state(state_ptr(new NumericState()));
            break;
            
        case kOperationSymbol:
            automation.set_state(state_ptr(new OperationState()));
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
            // if (automation.buffer().length() != 1)
            //     automation.set_state(state_ptr(new ErrorState()));
            // else
                automation.clear_buffer();
            break;
     
        default:
            throw Syntax_error(automation.line());
            // automation.set_state(state_ptr(new ErrorState()));
    }    
}

void AlphanumericState::next_state(LexingAutomation &automation, char symbol) {
    switch (determine_symbol_type(symbol)) {
        case kUnderscore:
        case kLetter:
        case kDigit: break;
            
        case kOperationSymbol:
            automation.set_result(Lexeme(handle_alphanumeric(automation.get_buffer_prefix()), automation.get_buffer_prefix(), automation.line()));
            automation.set_buffer_to_last_char();
            automation.set_state(state_ptr(new OperationState()));
            break;
            
        case kSeparator:
            automation.set_result(Lexeme(handle_alphanumeric(automation.get_buffer_prefix()), automation.get_buffer_prefix(), automation.line()));
            automation.set_result(Lexeme(handle_separator(automation.buffer().back()),  "", automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
        
        case kLinefeed:
            automation.set_result(Lexeme(handle_alphanumeric(automation.get_buffer_prefix()), automation.get_buffer_prefix(), automation.line()));
            automation.set_result(Lexeme(kEndofLine, "", automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
            
        case kWhitespace:
            automation.set_result(Lexeme(handle_alphanumeric(automation.get_buffer_prefix()), automation.get_buffer_prefix(), automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
                        
        default:
            throw Syntax_error(automation.line());
            // automation.set_state(state_ptr(new ErrorState()));
    }
}

void NumericState::next_state(LexingAutomation &automation, char symbol) {
    switch (determine_symbol_type(symbol)) {
        case kDigit:
            break;
            
        case kOperationSymbol:
            automation.set_result(Lexeme(kNumber, automation.get_buffer_prefix(), automation.line()));
            automation.set_buffer_to_last_char();
            automation.set_state(state_ptr(new OperationState()));
            break;
            
        case kSeparator:
            automation.set_result(Lexeme(kNumber, automation.get_buffer_prefix(), automation.line()));
            automation.set_result(Lexeme(handle_separator(automation.buffer().back()), "", automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
        
        case kLinefeed:
            automation.set_result(Lexeme(kNumber, automation.get_buffer_prefix(), automation.line()));
            automation.set_result(Lexeme(kEndofLine, "", automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
            
        case kWhitespace:
            automation.set_result(Lexeme(kNumber, automation.get_buffer_prefix(), automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
            
        default:
            throw Syntax_error(automation.line());
            // automation.set_state(state_ptr(new ErrorState()));
    }
}

void OperationState::next_state(LexingAutomation& automation, char symbol) {
    switch (determine_symbol_type(symbol)) {
        case kLetter:
            automation.set_result(Lexeme(handle_operation(automation.get_buffer_prefix()), "", automation.line()));
            automation.set_buffer_to_last_char();
            automation.set_state(state_ptr(new AlphanumericState()));
            break;

        case kDigit:
            automation.set_result(Lexeme(handle_operation(automation.get_buffer_prefix()), "", automation.line()));
            automation.set_buffer_to_last_char();
            automation.set_state(state_ptr(new NumericState()));
            break;
            
        case kOperationSymbol:
            if (mapTypesOp.count(automation.buffer()) == 0) {
                automation.set_result(Lexeme(handle_operation(automation.get_buffer_prefix()), "", automation.line()));
                string lexeme_symbol(1, automation.buffer().back());
                automation.set_result(Lexeme(handle_operation(lexeme_symbol), "" , automation.line()));
                automation.clear_buffer();
                automation.set_state(state_ptr(new EmptyState()));
            } else {
                automation.set_result(Lexeme(handle_operation(automation.buffer()), "", automation.line()));
                automation.clear_buffer();
                automation.set_state(state_ptr(new EmptyState()));
            }
            break;
            
        case kSeparator:
            automation.set_result(Lexeme(handle_operation(automation.get_buffer_prefix()), "", automation.line()));
            automation.set_result(Lexeme(handle_separator(automation.buffer().back()), "", automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
            
        case kWhitespace:
            automation.set_result(Lexeme(handle_operation(automation.get_buffer_prefix()), "", automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
            
        default:
            throw Syntax_error(automation.line());
            // automation.set_state(state_ptr(new ErrorState()));
    }
}
