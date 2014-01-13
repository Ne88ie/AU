#ifndef Instruction_H_
#define Instruction_H_

#include <vector>
#include <string>

#include "Declarations.h"
#include "Visitor.h"

using std::vector;
using std::string;


class Instruction {
public:
    Instruction(size_t const line): m_line(line) { }
    virtual ~Instruction() { }
    
    size_t line() const {
        return m_line;
    }
    virtual int accept_visit (Visitor* visitor) const = 0;
    
private:
    size_t m_line;
};


class Assignment: public Instruction {
public:
    Assignment(size_t const line, string const& name, instruction_ptr expression):
        Instruction(line),
        m_name(name),
        m_expression(expression) { }
    
    string const& name() const {
        return m_name;
    }
    instruction_ptr const expression() const {
        return m_expression;
    }
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }
    
private:
    string m_name;
    instruction_ptr m_expression;
};


class Constant: public Instruction {
public:
    Constant(size_t const line, int value): Instruction(line), m_value(value) { }
    
    int value() const {
        return m_value;
    }
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }
    
private:
    int m_value;
};


class Variable: public Instruction {
public:
    Variable(size_t const line, string const& name): Instruction(line), m_name(name) { }
    
    string const& name() const {
        return m_name;
    }
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }
    
private:
    string m_name;
    
};


class Function_Call: public Instruction {
public:
    Function_Call(size_t const line, string const& name, instructions arguments):
        Instruction(line),
        m_name(name),
        m_arguments(arguments) { }
    
    string const& name() const {
        return m_name;
    }
    instructions const& arguments() const {
        return m_arguments;
    }
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }
    
private:
    string m_name;
    instructions m_arguments;
};


class Arithmetic_Operation: public Instruction {
public:
    enum Arithm_Operation {
        kAdd,
        kSub,
        kMul,
        kDiv
    } m_operation;
    Arithmetic_Operation(size_t const line,
                                   Arithm_Operation operation,
                                   instruction_ptr left_operand,
                                   instruction_ptr right_operand):
        Instruction(line),
        m_operation(operation),
        m_left_operand(left_operand),
        m_right_operand(right_operand) { }
    
    Arithm_Operation operation() const {
        return m_operation;
    }
    instruction_ptr left_operand() const {
        return m_left_operand;
    }
    instruction_ptr right_operand() const {
        return m_right_operand;
    }
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }
    
private:
    instruction_ptr m_left_operand;
    instruction_ptr m_right_operand;
};


class Conditional: public Instruction {
public:
    enum Logical_Operation {
        kEqual,
        kNotEqual,
        kLess,
        kGreater,
        kLessEqual,
        kGreaterEqual
    } m_operation;
    Conditional(size_t const line,
                           Logical_Operation operation,
                           instruction_ptr left_operand,
                           instruction_ptr right_operand):
        Instruction(line),
        m_operation(operation),
        m_left_operand(left_operand),
        m_right_operand(right_operand) { }
    
    Logical_Operation operation() const {
        return m_operation;
    }
    instruction_ptr left_operand() const {
        return m_left_operand;
    }
    instruction_ptr right_operand() const {
        return m_right_operand;
    }
    
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }
    
private:
    instruction_ptr m_left_operand;
    instruction_ptr m_right_operand;
};


class Read: public Instruction {
public:
    Read(size_t const line, string const& variable):
        Instruction(line),
        m_variable(variable) { }
        
    string const& variable() const {
        return m_variable;
    }
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }
    
private:
    string m_variable;
};


class Print: public Instruction {
public:
    Print(size_t const line, instruction_ptr expression):
        Instruction(line),
        m_expression(expression) { }

    instruction_ptr expression() const {
        return m_expression;
    } 
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }
    
private:
    instruction_ptr m_expression;
};


class Return: public Instruction {
public:
    Return(size_t const line, instruction_ptr expression):
        Instruction(line),
        m_expression(expression) { }
        
    instruction_ptr expression() const {
        return m_expression;
    }    
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }
    
private:
    instruction_ptr m_expression;
};


class Instruction_Block: public Instruction {
public:
    Instruction_Block(size_t const line, instructions const& block):
        Instruction(line),
        m_block(block) { }
    
    instructions const& block() const {
        return m_block;
    }
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }
    
private:
    instructions m_block;
};


class If_Block: public Instruction_Block {
public:
    If_Block(size_t const line, instructions const& block, instruction_ptr condition):
        Instruction_Block(line, block),
        m_condition(condition) { }

    instruction_ptr condition() const {
        return m_condition;
    }
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }
    
private:
    instruction_ptr m_condition;
};


class While_Block: public Instruction_Block {
public:
    While_Block(size_t const line, instructions const& block, instruction_ptr condition):
        Instruction_Block(line, block),
        m_condition(condition) { }

    instruction_ptr condition() const {
        return m_condition;
    }
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }
    
private:
    instruction_ptr m_condition;
};


class Function: public Instruction_Block {
public:
    Function(size_t const line, instructions const& block, string const& name, vector<string> const& arguments):
        Instruction_Block(line, block),
        m_name(name),
        m_arguments(arguments) { }
    
    string const& name() const {
        return m_name;
    }
    vector<string> const& arguments() const {
        return m_arguments;
    }    
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }

private:
    string m_name;
    vector<string> m_arguments;
};


class Program: public Instruction_Block {
public:
    Program(size_t const line, instructions const& body, instructions& functions):
        Instruction_Block(line, body),
        m_functions(functions) { }

    instructions const& functions() const {
        return m_functions;
    }
    int accept_visit(Visitor* visitor) const {
        return visitor->visit(*this);
    }

private:
    instructions m_functions;
};


#endif
