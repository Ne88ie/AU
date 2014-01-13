#include "Lexer.h"
#include "Exception.h"

using std::cout;
using std::unordered_map;


string Lexer::get_buffer_prefix() {
    return buffer.length() < 1 ? "" : buffer.substr(0, buffer.length() - 1);
}

void Lexer::set_buffer_to_last_char() {
    buffer = buffer.substr(buffer.length() - 1);
}

Symbol_Types Lexer::determine_symbol_type(char symbol) {
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

const std::unordered_map<string, Lexeme_Types> map_types_alnumerics = {
    {"if", kIfKeyword},
    {"end", kEndKeyword},
    {"def", kDefKeyword},
    {"read", kReadKeyword},
    {"print", kPrintKeyword},
    {"while", kWhileKeyword},
    {"return", kReturnKeyword}
};

Lexeme Lexer::alphanumeric_lexeme() {
    Lexeme_Types type = map_types_alnumerics.count(get_buffer_prefix()) != 0 ? map_types_alnumerics.find(get_buffer_prefix())->second : kId;
    return Lexeme(type, get_buffer_prefix(), line);
}

const std::unordered_map<char, Lexeme_Types> map_types_seps = {
    {',', kComma},
    {':', kColon},
    {'(', kLeftBracket},
    {')', kRightBracket}
};

Lexeme Lexer::separator_lexeme() {
    Lexeme_Types type = map_types_seps.count(buffer.back()) != 0 ? map_types_seps.find(buffer.back())->second : kUnknownLexeme;
    return Lexeme(type, "", line);
}

const std::unordered_map<string, Lexeme_Types> map_types_operators = {
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

Lexeme Lexer::operation_lexeme(string operation) {
    Lexeme_Types type = map_types_operators.count(operation) != 0 ? map_types_operators.find(operation)->second : kUnknownLexeme;
    return Lexeme(type, "", line);
}

void Lexer::empty_state_to_next_state(Symbol_Types type) {
    switch (type) {
        case kLetter:
            current_state = alphanumeric_state;
            break;
            
        case kDigit:
            current_state = numeric_state;
            break;
            
        case kOperationSymbol:
            current_state = operation_state;
            break;
            
        case kSeparator:
            result.push_back(separator_lexeme());
            buffer.clear();
            break;
            
        case kLinefeed:
            result.push_back(Lexeme(kEndofLine, "", line));
            buffer.clear();
            break;
            
        case kWhitespace:
            buffer.clear();
            break;
     
        default:
            throw Syntax_error(line);
    }    
}


void Lexer::alphanumeric_state_to_next_state(Symbol_Types type) {
    switch (type) {
        case kUnderscore:
        case kLetter:
        case kDigit: break;
            
        case kOperationSymbol:
            result.push_back(alphanumeric_lexeme());
            set_buffer_to_last_char();
            current_state = operation_state;
            break;
            
        case kSeparator:
            result.push_back(alphanumeric_lexeme());
            result.push_back(separator_lexeme());
            buffer.clear();
            current_state = empty_state;
            break;
        
        case kLinefeed:
            result.push_back(alphanumeric_lexeme());
            result.push_back(Lexeme(kEndofLine, "", line));
            buffer.clear();
            current_state = empty_state;
            break;
            
        case kWhitespace:
            result.push_back(alphanumeric_lexeme());
            buffer.clear();
            current_state = empty_state;
            break;
                        
        default:
            throw Syntax_error(line);
    }
}

void Lexer::numeric_state_to_next_state(Symbol_Types type) {
    switch (type) {
        case kDigit:
            break;
            
        case kOperationSymbol:
            result.push_back(Lexeme(kNumber, get_buffer_prefix(), line));
            set_buffer_to_last_char();
            current_state = operation_state;
            break;
            
        case kSeparator:
            result.push_back(Lexeme(kNumber, get_buffer_prefix(), line));
            result.push_back(separator_lexeme());
            buffer.clear();
            current_state = empty_state;
            break;
        
        case kLinefeed:
            result.push_back(Lexeme(kNumber, get_buffer_prefix(), line));
            result.push_back(Lexeme(kEndofLine, "", line));
            buffer.clear();
            current_state = empty_state;
            break;
            
        case kWhitespace:
            result.push_back(Lexeme(kNumber, get_buffer_prefix(), line));
            buffer.clear();
            current_state = empty_state;
            break;
            
        default:
            throw Syntax_error(line);
    }
}

void Lexer::operation_state_to_next_state(Symbol_Types type) {
    switch (type) {
        case kLetter:
            result.push_back(operation_lexeme(get_buffer_prefix()));
            set_buffer_to_last_char();
            current_state = alphanumeric_state;
            break;

        case kDigit:
            result.push_back(operation_lexeme(get_buffer_prefix()));
            set_buffer_to_last_char();
            current_state = numeric_state;
            break;
            
        case kOperationSymbol:
            if (map_types_operators.count(buffer) == 0) {
                result.push_back(operation_lexeme(get_buffer_prefix()));
                string lexeme_symbol(1, buffer.back());
                result.push_back(operation_lexeme(lexeme_symbol));
                buffer.clear();
                current_state = empty_state;
            } else {
                result.push_back(operation_lexeme(buffer));
                buffer.clear();
                current_state = empty_state;
            }
            break;
            
        case kSeparator:
            result.push_back(operation_lexeme(get_buffer_prefix()));
            result.push_back(separator_lexeme());
            buffer.clear();
            current_state = empty_state;
            break;
        
        case kLinefeed:
            throw Syntax_error(line);
            break;
            
        case kWhitespace:
            result.push_back(operation_lexeme(get_buffer_prefix()));
            buffer.clear();
            current_state = empty_state;
            break;
            
        default:
            throw Syntax_error(line);
    }
}

void Lexer::next_state(Symbol_Types type) {
    switch (current_state) {
        case empty_state:
            empty_state_to_next_state(type);
            break;
        case alphanumeric_state:
            alphanumeric_state_to_next_state(type);
            break;
        case numeric_state:
            numeric_state_to_next_state(type);
            break;
        case operation_state:
            operation_state_to_next_state(type);
            break;
    }
}

Lexer::Lexer(string const & file_path):
        line(1),
        buffer(""),
        current_state(empty_state),
        result(vector<Lexeme>()) {
    ifstream input_stream(file_path);
    if(!input_stream)
        cout << "File " << file_path << " not found.\n";
    else 
        tokenize(input_stream);
}

void Lexer::tokenize(ifstream &input_stream) {
    string input_line;
    while (getline(input_stream, input_line)) {
        for (size_t i = 0; i < input_line.length(); ++i) {
            if (input_line[i] == '#') break;
            buffer += input_line[i];
            next_state(determine_symbol_type(input_line[i]));
        }
        buffer += '\n';
        next_state(kLinefeed);
        ++line;    
    }
    result.push_back(Lexeme(kEndofFile, "", line));
}

vector<Lexeme> const Lexer::get_result() {
    vector<Lexeme> clean_result;  
    for (size_t i = 0; i < result.size(); ++i) {
        clean_result.push_back(result[i]);
        if (result[i].type() == kEndofLine)
            while (i + 1 < result.size() && result[i + 1].type() == kEndofLine)
                ++i;   
    }
    return clean_result;
}
