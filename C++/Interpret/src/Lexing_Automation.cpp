#include "Lexing_Automation.h"


Lexing_Automation::Lexing_Automation():
    m_line(1),
    m_buffer(""),
    m_state(new Empty_State()),
    m_result(vector<Lexeme>()) { }

void Lexing_Automation::set_result(Lexeme lexeme) {
    m_result.push_back(lexeme);
}

void Lexing_Automation::set_state(state_ptr state) {
    m_state = state;
}

void Lexing_Automation::clear_buffer() {
    m_buffer = "";
}

void Lexing_Automation::set_buffer(string value) {
    m_buffer = value;
}

string Lexing_Automation::get_buffer_prefix() const {
    return m_buffer.length() < 1 ? "" : m_buffer.substr(0, m_buffer.length() - 1);
}

void Lexing_Automation::set_buffer_to_last_char() {
    m_buffer = m_buffer.substr(m_buffer.length() - 1);
}

void Lexing_Automation::next_symbol(char symbol) {
    m_buffer += symbol;
    m_state->next_state(*this, symbol);
}

void Lexing_Automation::increment_line() {
    ++m_line; 
}

