#include <iostream>

#include "Evaluator.h"

using std::cin;
using std::cout;
using std::endl;


Evaluator::Evaluator(Program const& program): m_program(program) { }

void Evaluator::execute_program() {
    visit(m_program);
}

int Evaluator::visit(Constant const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;
    
    return instruction.value();
}

int Evaluator::visit(Variable const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;

    variable_scope::iterator variable = m_variable_scopes.top().find(instruction.name());
    if (variable == m_variable_scopes.top().end()) {
        ErrorHandler::report_undefined_variable(instruction.line(), instruction.name());
        return 0;
    }
    return variable->second;
}

int Evaluator::visit(Program const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;

    variable_scope program_scope;
    m_variable_scopes.push(program_scope);

    for (instructions::const_iterator it = instruction.functions().begin(); it != instruction.functions().end(); ++it)
        (*it)->accept_visit(this);

    instruction.InstructionBlock::accept_visit(this);
    m_variable_scopes.pop();
    return m_result;
}

int Evaluator::visit(Function const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;

    m_function_scope[instruction.name()] = &instruction;
    return 0;
}

int Evaluator::visit(Assignment const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;

    int value = instruction.expression()->accept_visit(this);
    m_variable_scopes.top()[instruction.name()] = value;
    return m_result;
}

int Evaluator::visit(FunctionCall const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;

    function_scope::const_iterator func = m_function_scope.find(instruction.name());
    if (func == m_function_scope.end()) {
        ErrorHandler::report_undefined_function(instruction.line(), instruction.name());
        return 0;
    }
    
    Function const* function_to_call = func->second;
    
    if (function_to_call->arguments().size() != instruction.arguments().size()) {
        ErrorHandler::report_arguments_number_mismatch(instruction.line(), instruction.name());
        return 0;
    }
    
    variable_scope function_to_call_scope;
    
    for (size_t i = 0; i != instruction.arguments().size(); ++i)
        function_to_call_scope[function_to_call->arguments()[i]] = instruction.arguments()[i]->accept_visit(this);
    
    m_variable_scopes.push(function_to_call_scope);
    function_to_call->InstructionBlock::accept_visit(this);
    m_variable_scopes.pop();
    int result = m_result;
    m_result = 0;
    m_has_returned = false;
    return result;
}

int Evaluator::visit(ArithmeticOperation const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;

    int left = instruction.left_operand()->accept_visit(this);
    int right = instruction.right_operand()->accept_visit(this);
    
    switch (instruction.operation()) {
        case ArithmeticOperation::kAdd:
            return left + right;   
        case ArithmeticOperation::kSub:
            return left - right;
        case ArithmeticOperation::kMul:
            return left * right;
        case ArithmeticOperation::kDiv:
            if (right == 0) {
                ErrorHandler::report_division_by_zero(instruction.line());
                return 0;
            }
            return left / right;
        default:
            return 0;
    }
}

int Evaluator::visit(Read const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;

    cin >> m_variable_scopes.top()[instruction.variable()];
    return m_result;
}

int Evaluator::visit(Print const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;

    int result = instruction.expression()->accept_visit(this);
    if (!ErrorHandler::is_ok()) return 0;

    cout << result << endl;
    return m_result;
}

int Evaluator::visit(Conditional const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;

    int left = instruction.left_operand()->accept_visit(this);
    int right = instruction.right_operand()->accept_visit(this);
    
    switch (instruction.operation()) {
        case Conditional::kEqual:
            return left == right;
        case Conditional::kNotEqual:
            return left != right;
        case Conditional::kLess:
            return left < right;
        case Conditional::kGreater:
            return left > right;
        case Conditional::kLessEqual:
            return left <= right;
        case Conditional::kGreaterEqual:
            return left >= right;
        default:
            return 0;
    }
}

int Evaluator::visit(IfBlock const& instruction) {
    if (!ErrorHandler::is_ok()) return 0; 

    if (instruction.condition()->accept_visit(this))
        instruction.InstructionBlock::accept_visit(this);

    return m_result;
}

int Evaluator::visit(WhileBlock const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;

    while (instruction.condition()->accept_visit(this))
        instruction.InstructionBlock::accept_visit(this);

    return m_result;
}

int Evaluator::visit(Return const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;

    m_result = instruction.expression()->accept_visit(this);
    m_has_returned = true;
    return m_result;
}

int Evaluator::visit(InstructionBlock const& instruction) {
    if (!ErrorHandler::is_ok()) return 0;

    for (instructions::const_iterator it = instruction.block().begin(); it != instruction.block().end() && !m_has_returned; ++it)
        (*it)->accept_visit(this);

    return m_result;
}
