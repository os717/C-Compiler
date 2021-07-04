#ifndef AST_LOGIC_HPP
#define AST_LOGIC_HPP

#include <iostream>
#include <string>
#include <vector>
#include "cont.hpp"
#include "ast_leaf.hpp"

class LEAF;
typedef const LEAF *Pointer_LEAF;

class Log_And : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Log_And(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        v_left->PYTHON_t(stream, SCP);
        stream << " and ";
        v_right->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\n\tsltu $" << dReg << ",$" << dReg << ",1\n";
        stream << "\txori $" << dReg << ",$" << dReg << ",1\n";
        stream << "\tsltu $" << CONT.R2 << ",$" << CONT.R2 << ",1\n";
        stream << "\txori $" << CONT.R2 << ",$" << CONT.R2 << ",1\n";
        stream << "\tand $" << dReg << ",$" << dReg << ",$" << CONT.R2 << "\n";
        CONT.R2Decr();
    }
};

class Log_Or : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Log_Or(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        v_left->PYTHON_t(stream, SCP);
        stream << " or ";
        v_right->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\n\tsltu $" << dReg << ",$" << dReg << ",1\n";
        stream << "\txori $" << dReg << ",$" << dReg << ",1\n";
        stream << "\tsltu $" << CONT.R2 << ",$" << CONT.R2 << ",1\n";
        stream << "\txori $" << CONT.R2 << ",$" << CONT.R2 << ",1\n";
        stream << "\tor $" << dReg << ",$" << dReg << ",$" << CONT.R2 << "\n";
        CONT.R2Decr();
    }
};

class Log_Not : public LEAF
{
private:
    const Pointer_LEAF val;

public:
    Log_Not(const Pointer_LEAF _val) : val(_val) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Log_Not not in python spec\n";
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        val->C90_COMP(stream, CONT);
        stream << "\n\tsltu $" << dReg << ",$" << dReg << ",1\n";
        CONT.R2Decr();
    }
};

class Bit_Or : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Bit_Or(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        v_left->PYTHON_t(stream, SCP);
        stream << " or ";
        v_right->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\tor $" << dReg << ",$" << dReg << ",$" << CONT.R2 << "\n";
        CONT.R2Decr();
    }
};

class Bit_Xor : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Bit_Xor(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Bit_Xor not in python translator spec" << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\txor $" << dReg << ",$" << dReg << ",$" << CONT.R2 << "\n";
        CONT.R2Decr();
    }
};

class Bit_And : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Bit_And(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Bit_And not in python translator spec" << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\tand $" << dReg << ",$" << dReg << ",$" << CONT.R2 << "\n";
        CONT.R2Decr();
    }
};

class Bit_Not : public LEAF
{
private:
    const Pointer_LEAF val;

public:
    Bit_Not(const Pointer_LEAF _val) : val(_val) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Bit_Not not in python translator spec" << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        val->C90_COMP(stream, CONT);
        CONT.R2Incr();
        stream << "\tli $" << CONT.R2 << ",-1\n";
        stream << "\tsub $" << dReg << ",$" << CONT.R2 << ",$" << dReg << "\n";
        CONT.R2Decr();
    }
};

class Equ : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Equ(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        v_left->PYTHON_t(stream, SCP);
        stream << "==";
        v_right->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\txor $" << dReg << ",$" << CONT.R2 << ",$" << dReg << "\n";
        stream << "\tsltu $" << dReg << ",$" << dReg << ",1\n";
        CONT.R2Decr();
    }
};

class N_Equ : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    N_Equ(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "N_Equ not in python translator spec" << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\txor $" << dReg << ",$" << CONT.R2 << ",$" << dReg << "\n";
        stream << "\tsltu $" << dReg << ",$" << dReg << ",1\n";
        stream << "\txori $" << dReg << ",$" << dReg << ",1\n";
        CONT.R2Decr();
    }
};

class Less : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Less(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        v_left->PYTHON_t(stream, SCP);
        stream << "<";
        v_right->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\tslt $" << dReg << ",$" << dReg << ",$" << CONT.R2 << "\n";
        CONT.R2Decr();
    }
};

class Great : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Great(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Great not in python translator spec" << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\tslt $" << dReg << ",$" << CONT.R2 << ",$" << dReg << "\n";
        CONT.R2Decr();
    }
};

class Less_Equ : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Less_Equ(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Less_Equ not in python translator spec" << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\tslt $" << dReg << ",$" << CONT.R2 << ",$" << dReg << "\n";
        stream << "\txori $" << dReg << ",1\n";
        CONT.R2Decr();
    }
};

class Great_Equ : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Great_Equ(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Great_Equ not in python translator spec" << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\tslt $" << dReg << ",$" << dReg << ",$" << CONT.R2 << "\n";
        stream << "\txori $" << dReg << ",1\n";
        CONT.R2Decr();
    }
};

#endif
