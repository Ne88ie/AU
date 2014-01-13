#ifndef Visitor_H
#define Visitor_H

class Program;
class Function;
class Constant;
class Variable;
class Assignment;
class Function_Call;
class Arithmetic_Operation;
class Read;
class Print;
class Conditional;
class If_Block;
class While_Block;
class Return;
class Instruction_Block;


class Visitor {
public:
    virtual int visit(Constant const&) = 0;
    virtual int visit(Variable const&) = 0;
    virtual int visit(Program const&) = 0;
    virtual int visit(Function const&) = 0;
    virtual int visit(Assignment const&) = 0;
    virtual int visit(Function_Call const&) = 0;
    virtual int visit(Arithmetic_Operation const&) = 0;
    virtual int visit(Read const&) = 0;
    virtual int visit(Print const&) = 0;
    virtual int visit(Conditional const&) = 0;
    virtual int visit(If_Block const&) = 0;
    virtual int visit(While_Block const&) = 0;
    virtual int visit(Return const&) = 0;
    virtual int visit(Instruction_Block const&) = 0;
    
    virtual ~Visitor() { }
};

#endif
