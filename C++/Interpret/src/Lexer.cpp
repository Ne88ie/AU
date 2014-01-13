#include "Lexer.h"


Lexer::Lexer(string const & file_path): m_automation(new LexingAutomation()) {
        ifstream input_stream(file_path);
        if(!input_stream)
            cout << "File " << file_path << " not found.\n";
        else
            tokenize(input_stream);
    }

void Lexer::tokenize(ifstream &input_stream) {
    string line;
    while (getline(input_stream, line)) {
        for (size_t i = 0; i < line.length(); ++i) {
            if (line[i] == '#') break;
            m_automation->next_symbol(line[i]);
        }
        m_automation->next_symbol('\n');
        m_automation->increment_line();    
    }
    m_automation->set_result(Lexeme(kEndofFile, "", m_automation->line()));
}

vector<Lexeme> Lexer::get_result() {
    vector<Lexeme> result = m_automation->result();
    vector<Lexeme> clean_result;  
    for (size_t i = 0; i < result.size(); ++i) {
        clean_result.push_back(result[i]);
        if (result[i].type() == kEndofLine)
            while (i + 1 < result.size() && result[i + 1].type() == kEndofLine) ++i;   
    }
    return clean_result;
}
