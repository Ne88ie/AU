#include <unordered_map>

#include "Lexer.h"
#include "Exception.h"

using std::cout;
using std::unordered_map;


Lexer::Lexer(string const & file_path): line_number(1) {
    ifstream input_stream(file_path);

    if(!input_stream)
        cout << "File " << file_path << " not found.\n";
    else {
        for (string input_line; getline(input_stream, input_line); ++line_number) {       
            if (input_line.length() != 0)
                process_line(input_line);        
        }
        result.push_back(Lexeme(kEndofFile, "", line_number));
    }
}

void Lexer::get_operation(string::const_iterator &it, Lexeme &current_lexeme) {
        switch (*it) {
        case ':':
            current_lexeme.type = kColon;
            break;
        case ',':
            current_lexeme.type = kComma;
            break;
        case '+':
            current_lexeme.type = kAddition;
            break;
        case '-':
            current_lexeme.type = kSubtraction;
            break;
        case '*':
            current_lexeme.type = kMultiplication;
            break;
        case '/':
            current_lexeme.type = kDivision;
            break;
        case '(':
            current_lexeme.type = kLeftBracket;
            break;
        case ')':
            current_lexeme.type = kRightBracket;
            break;
        case '<':
            current_lexeme.type = kLessOperation;
            if (*(it + 1) == '=') {
                current_lexeme.type = kLessEqualOperation;
                ++it;
            }
            break;
        case '>':
            current_lexeme.type = kGreaterOperation;
            if (*(it + 1) == '=') {
                current_lexeme.type = kGreaterEqualOperation;
                ++it;
            }
            break;
        case '=':
            if (*(it + 1) == '=') {
                current_lexeme.type = kEqualOperation;
                ++it;
            } else
                current_lexeme.type = kAssignment;
            break;
        case '!':
            if (*(it + 1) == '=') {
                current_lexeme.type = kNotEqualOperation;
                ++it;
            } else
                throw Syntax_error(line_number);
            break;
    }
}


const unordered_map<string, Lexeme_Types> map_identifier_types = {
    {"if", kIfKeyword},
    {"end", kEndKeyword},
    {"def", kDefKeyword},
    {"read", kReadKeyword},
    {"print", kPrintKeyword},
    {"while", kWhileKeyword},
    {"return", kReturnKeyword}
};


void Lexer::get_identifier(string::const_iterator &it, Lexeme &current_lexeme) {
    if (isalpha(*it)) {
        string identifier;

        while (isalpha(*it) || isdigit(*it) || *it == '_')
            identifier += *(it++);
        --it;

        current_lexeme = map_identifier_types.count(identifier) != 0 ? map_identifier_types.find(identifier)->second : kId;
        current_lexeme.value = identifier;
    }
}


void Lexer::get_number(string::const_iterator &it, Lexeme &current_lexeme) {
    if (isdigit(*it)) {
        string number;
        while (isdigit(*it))
            number += *(it++);

        if (isalpha(*it) || *it == '_')
            throw Syntax_error(line_number);

        --it;
        current_lexeme.type = kNumber;
        current_lexeme.value = number;
    }
}


void Lexer::process_line(string const &line) {
    for (string::const_iterator it = line.begin(); it != line.end(); ++it) {
        if (*it == '#')
            break;
        if (isspace(*it))
            continue;

        Lexeme current_lexeme(kUnknownLexeme, "", line_number);

        get_operation(it, current_lexeme);

        if (current_lexeme.type == kUnknownLexeme)
            get_identifier(it, current_lexeme);

        if (current_lexeme.type == kUnknownLexeme)
            get_number(it, current_lexeme);

        if (current_lexeme.type == kUnknownLexeme) 
            throw Syntax_error(line_number);
        
        current_lexeme.line = line_number;
        result.push_back(current_lexeme);
    }
    result.push_back(Lexeme(kEndofLine, "", line_number));
}


vector<Lexeme> const Lexer::get_result() {
    vector<Lexeme> clean_result;  
    for (size_t i = 0; i < result.size(); ++i) {
        clean_result.push_back(result[i]);
        if (result[i].type == kEndofLine)
            while (i + 1 < result.size() && result[i + 1].type == kEndofLine)
                ++i;   
    }
    return clean_result;
}
