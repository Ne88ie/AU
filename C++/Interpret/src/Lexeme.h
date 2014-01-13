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

class Lexeme {
public:
    Lexeme(Lexeme_Types type, string value, size_t const line):
        m_type(type),
        m_value(value),
        m_line(line) { }

    Lexeme_Types type() const {
        return m_type;
    }
    string value() const {
        return m_value;
    }
    size_t line() const {
        return m_line;
    }

private:
    Lexeme_Types m_type;
    string m_value;
    size_t m_line;
};

#endif 
