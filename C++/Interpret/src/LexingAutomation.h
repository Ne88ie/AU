#ifndef LexingAutomation_H_
#define LexingAutomation_H_

#include <string>
#include <vector>
#include <memory>
#include <cctype>
#include <unordered_map>

#include "Lexeme.h"

class LexerState;

using std::string;
using std::vector;
using std::unordered_map;

typedef std::shared_ptr<LexerState> state_ptr;


enum SymbolTypes {
    kUnknownType = -1,
    kLetter,
    kUnderscore,
    kDigit,
    kOperationSymbol,
    kSeparator,
    kLinefeed,
    kWhitespace
};


class LexingAutomation {
public:
    LexingAutomation();
    void set_state(state_ptr);
    void set_result(Lexeme);
    
    void set_buffer(string);
    void set_buffer_to_last_char();
    void clear_buffer();
    string get_buffer_prefix() const;
    
    void next_symbol(char symbol);
    void next_line();
    
    string const& buffer() const {
        return m_buffer;
    }
    size_t line() const {
        return m_line;
    }
    vector<Lexeme> const& result() const {
        return m_result;
    }
    
private:
    LexingAutomation(LexingAutomation const&);
    LexingAutomation& operator=(LexingAutomation const&);
    
    SymbolTypes determine_symbol_type(char symbol);
    size_t m_line;
    string m_buffer;
    state_ptr m_state;
    vector<Lexeme> m_result;
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

Lexeme handle_separator(char separator, size_t const line);
Lexeme handle_number(string number, size_t const line);
Lexeme handle_alphanumeric(string alphanumeric, size_t const line);
Lexeme handle_operation(string operation, size_t const line);
Lexeme end_of_line(size_t const line);

#endif
