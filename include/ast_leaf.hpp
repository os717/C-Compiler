#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <iostream>
#include <string>
#include <vector>
#include "cont.hpp"

extern std::vector<std::string> variables_global;
class LEAF;
typedef const LEAF *Pointer_LEAF;
extern const Pointer_LEAF DO_PARSE(const char file[]);

class LEAF
{
public:
    virtual ~LEAF() {}
    virtual void PYTHON_t(std::ostream &stream, int &SCP) const = 0;
    virtual void C90_COMP(std::ostream &stream, CONT &CONT) const = 0;
    virtual std::string fetchlab() const {}
    virtual std::string fetchnam() const {}
};

#endif
