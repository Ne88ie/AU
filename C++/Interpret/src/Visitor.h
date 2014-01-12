#ifndef Visitor_H
#define Visitor_H

class Program;
class Function;
class Constant;
class Variable;
class Assignment;
class FunctionCall;
class ArithmeticOperation;
class Read;
class Print;
class Conditional;
class IfBlock;
class WhileBlock;
class Return;
class InstructionBlock;


class Visitor {
public:
    virtual int visit(Constant const&) = 0;
    virtual int visit(Variable const&) = 0;
    virtual int visit(Program const&) = 0;
    virtual int visit(Function const&) = 0;
    virtual int visit(Assignment const&) = 0;
    virtual int visit(FunctionCall const&) = 0;
    virtual int visit(ArithmeticOperation const&) = 0;
    virtual int visit(Read const&) = 0;
    virtual int visit(Print const&) = 0;
    virtual int visit(Conditional const&) = 0;
    virtual int visit(IfBlock const&) = 0;
    virtual int visit(WhileBlock const&) = 0;
    virtual int visit(Return const&) = 0;
    virtual int visit(InstructionBlock const&) = 0;
    
    virtual ~Visitor() { }
};

#endif
