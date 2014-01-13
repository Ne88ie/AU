#include "LexingAutomation.h"


LexingAutomation::LexingAutomation():
    m_line(1),
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

void LexingAutomation::next_symbol(char symbol) {
    m_buffer += symbol;
    m_state->next_state(*this, symbol);
}

void LexingAutomation::increment_line() {
    ++m_line; 
}

