#include "ErrorHandler.h"
#include "Lexer.h"


Lexer::Lexer(istream_ptr input_stream):
    m_stream(input_stream),
    m_automation(new LexingAutomation()) { }

void Lexer::tokenize() {
    string line;
    while (true) {
        getline(*m_stream, line);
        if (!*m_stream) break;

        m_automation->next_line();
        for (size_t i = 0; i < line.length(); ++i) {
            if (line[i] == '#') break;
            m_automation->next_symbol(line[i]);
        }

        m_automation->next_symbol('\n');
        if (!ErrorHandler::is_ok()) return;
    }
}

vector<Lexeme> Lexer::get_result() {
    vector<Lexeme> result = m_automation->result();
    vector<Lexeme> clean_result;
    size_t i = 0;
    while (i < result.size()) {
        clean_result.push_back(result[i]);
        if (result[i].type() == kEndofLine) {
            ++i;
            while (i < result.size() && result[i].type() == kEndofLine) ++i;
        } else ++i;
    }
    return clean_result;
}
