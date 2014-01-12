#include "LexingAutomation.h"
#include "EmptyState.h"


LexingAutomation::LexingAutomation():
    m_line(0),
    m_buffer(""),
    m_state(new EmptyState()),
    m_result(vector<Lexeme>()) { }

void LexingAutomation::set_result(Lexeme lexeme) {
    m_result.push_back(lexeme);
}

void LexingAutomation::set_state(state_ptr state) {
    m_state = state;
}

void LexingAutomation::clear_buffer() {
    m_buffer = "";
}

void LexingAutomation::set_buffer(string value) {
    m_buffer = value;
}

string LexingAutomation::get_buffer_prefix() const {
    return m_buffer.length() < 1 ? "" : m_buffer.substr(0, m_buffer.length() - 1);
}

void LexingAutomation::set_buffer_to_last_char() {
    m_buffer = m_buffer.substr(m_buffer.length() - 1);
}

SymbolTypes LexingAutomation::determine_symbol_type(char symbol) {
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

void LexingAutomation::next_symbol(char symbol) {
    m_buffer += symbol;
    m_state->next_state(*this, determine_symbol_type(symbol));
}

void LexingAutomation::next_line() {
    ++m_line;
    clear_buffer();
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

Lexeme handle_separator(char separator, size_t const line) {
    LexemeTypes type = mapTypesSep.count(separator) != 0 ? mapTypesSep.find(separator)->second : kUnknownLexeme;
    return Lexeme(type, "", line);
}

Lexeme handle_number(string number, size_t line) {
    return Lexeme(kNumber, number, line);
}

Lexeme handle_alphanumeric(string alphanumeric, size_t const line) {
    LexemeTypes type = mapTypesAlnum.count(alphanumeric) != 0 ? mapTypesAlnum.find(alphanumeric)->second : kId;
    return Lexeme(type, alphanumeric, line);
}

Lexeme handle_operation(string operation, size_t const line) {
    LexemeTypes type = mapTypesOp.count(operation) != 0 ? mapTypesOp.find(operation)->second : kUnknownLexeme;
    return Lexeme(type, "", line);
}

Lexeme end_of_line(size_t const line) {
    return Lexeme(kEndofLine, "", line);
}
