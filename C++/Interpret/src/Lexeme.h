#ifndef Lexeme_H_
#define Lexeme_H_

#include <string>

using std::string;


enum Lexeme_Types {
    kUnknownLexeme = -1,
    kId,
    kNumber,
    kLeftBracket,
    kRightBracket,
    kComma,
    kColon,
    kEndofLine,
    kIfKeyword,
    kDefKeyword,
    kWhileKeyword,
    kReadKeyword,
    kPrintKeyword,
    kReturnKeyword,
    kEndKeyword,
    kAssignment,
    kLessOperation,
    kGreaterOperation,
    kLessEqualOperation,
    kGreaterEqualOperation,
    kEqualOperation,
    kNotEqualOperation,
    kAddition,
    kSubtraction,
    kDivision,
    kMultiplication,
    kEndofFile
};

struct Lexeme {
    Lexeme(Lexeme_Types new_type = kUnknownLexeme, string new_value = "", size_t const new_line_number = 0):
        type(new_type),
        value(new_value),
        line(new_line_number) { }

    Lexeme_Types type;
    string value;
    size_t line;
};

#endif 
