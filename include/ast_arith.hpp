#ifndef AST_ARITH_HPP
#define AST_ARITH_HPP

#include <iostream>
#include <string>
#include <vector>
#include "cont.hpp"
#include "ast_leaf.hpp"

class LEAF;
typedef const LEAF *Pointer_LEAF;

class Add : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Add(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        v_left->PYTHON_t(stream, SCP);
        stream << "+";
        v_right->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\taddu $" << dReg << ",$" << dReg << ",$" << CONT.R2 << "\n";
        CONT.R2Decr();
    }
};

class Sub : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Sub(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        v_left->PYTHON_t(stream, SCP);
        stream << "-";
        v_right->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\tsubu $" << dReg << ",$" << dReg << ",$" << CONT.R2 << "\n";
        CONT.R2Decr();
    }
};

class Mul : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Mul(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        v_left->PYTHON_t(stream, SCP);
        stream << "*";
        v_right->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\tmultu $" << dReg << ",$" << CONT.R2 << "\n";
        stream << "\tmflo $" << dReg << "\n";
        CONT.R2Decr();
    }
};

class Div : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Div(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Div not in python translator spec" << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\tdiv $" << dReg << ",$" << CONT.R2 << "\n";
        stream << "\tmflo $" << dReg << "\n";
        CONT.R2Decr();
    }
};

class Mod : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Mod(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Mod not in python translator spec" << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\tdiv $" << dReg << ",$" << CONT.R2 << "\n";
        stream << "\tmfhi $" << dReg << "\n";
        CONT.R2Decr();
    }
};

class Shift_L : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Shift_L(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Shift_L not in python translator spec" << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\tsll $" << dReg << ",$" << dReg << ",$" << CONT.R2 << "\n";
        CONT.R2Decr();
    }
};

class Shift_R : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Shift_R(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Shift_R not in python translator spec" << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        int dReg = CONT.R2;
        v_left->C90_COMP(stream, CONT);
        CONT.R2Incr();
        v_right->C90_COMP(stream, CONT);
        stream << "\tsra $" << dReg << ",$" << dReg << ",$" << CONT.R2 << "\n";
        CONT.R2Decr();
    }
};

class Unary_Minus : public LEAF
{
private:
    const Pointer_LEAF exp;

public:
    Unary_Minus(const Pointer_LEAF _exp) : exp(_exp) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "-";
        exp->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        exp->C90_COMP(stream, CONT);
        int R3 = CONT.R2;
        stream << "\tsub $" << R3 << ",$0,$" << R3 << "\n";
    }
};

class Incr_Pre : public LEAF
{
private:
    const Pointer_LEAF exp;

public:
    Incr_Pre(const Pointer_LEAF _exp) : exp(_exp) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Incr_Pre not in python spec\n";
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        exp->C90_COMP(stream, CONT);
        std::string NAM = exp->fetchnam();
        if (!CONT.gl_CHK(NAM))
        {
            stream << "\taddiu $" << CONT.R2 << ",$" << CONT.R2 << ",1\n";
            stream << "\tsw $" << CONT.R2 << "," << CONT.siz_fra - CONT.OfsFin(NAM) << "($fp)\n";
        }
    }
};

class Decr_Pre : public LEAF
{
private:
    const Pointer_LEAF exp;

public:
    Decr_Pre(const Pointer_LEAF _exp) : exp(_exp) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Incr_Pre not in python spec\n";
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        exp->C90_COMP(stream, CONT);
        std::string NAM = exp->fetchnam();
        if (!CONT.gl_CHK(NAM))
        {
            stream << "\taddiu $" << CONT.R2 << ",$" << CONT.R2 << ",-1\n";
            stream << "\tsw $" << CONT.R2 << "," << CONT.siz_fra - CONT.OfsFin(NAM) << "($fp)\n";
        }
    }
};

class Incr_Post : public LEAF
{
private:
    const Pointer_LEAF exp;

public:
    Incr_Post(const Pointer_LEAF _exp) : exp(_exp) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Incr_Post not in python spec\n";
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        exp->C90_COMP(stream, CONT);

        int R3 = CONT.R2;
        CONT.R2Incr();
        stream << "\tmove $" << CONT.R2 << ",$" << R3 << "\n";
        std::string NAM = exp->fetchnam();
        stream << "\taddiu $" << CONT.R2 << ",$" << CONT.R2 << ",1\n";
        stream << "\tsw $" << CONT.R2 << "," << CONT.siz_fra - CONT.OfsFin(NAM) << "($fp)\n";
        CONT.R2Decr();
    }
};

class Decr_Post : public LEAF
{
private:
    const Pointer_LEAF exp;

public:
    Decr_Post(const Pointer_LEAF _exp) : exp(_exp) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Incr_Pre not in python spec\n";
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        exp->C90_COMP(stream, CONT);

        int R3 = CONT.R2;
        CONT.R2Incr();
        stream << "\tmove $" << CONT.R2 << ",$" << R3 << "\n";
        std::string NAM = exp->fetchnam();
        stream << "\taddiu $" << CONT.R2 << ",$" << CONT.R2 << ",-1\n";
        stream << "\tsw $" << CONT.R2 << "," << CONT.siz_fra - CONT.OfsFin(NAM) << "($fp)\n";
        CONT.R2Decr();
    }
};

#endif
