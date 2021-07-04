#ifndef AST_STATE_HPP
#define AST_STATE_HPP

#include <iostream>
#include <string>
#include <vector>
#include "cont.hpp"
#include "ast_leaf.hpp"

class LEAF;
typedef const LEAF *Pointer_LEAF;

class If : public LEAF
{
private:
    const Pointer_LEAF cond;
    const Pointer_LEAF state;

public:
    If(const Pointer_LEAF _cond, const Pointer_LEAF _state) : cond(_cond), state(_state) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
		std::string tabs;
	for (int i = 0; i < SCP; i++) tabs += "\t";
	stream << tabs;
		stream << "if(";
        cond->PYTHON_t(stream, SCP);
        stream << "):\n";
        SCP++;
        state->PYTHON_t(stream, SCP);
        SCP--;
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        std::string LabelEnd = CONT.n_LAB("IF");
        cond->C90_COMP(stream, CONT);
        stream << "\tbeq $" << CONT.R2 << ",$"
               << "0,$" << LabelEnd << "\n";
        stream << "\tnop\n";
        state->C90_COMP(stream, CONT);
        stream << "$" << LabelEnd << ":\n";
    }
};

class If_Else : public LEAF
{
private:
    const Pointer_LEAF cond;
    const Pointer_LEAF t_stat;
    const Pointer_LEAF f_stat;

public:
    If_Else(const Pointer_LEAF _cond, const Pointer_LEAF _t_stat, const Pointer_LEAF _f_stat) : cond(_cond), t_stat(_t_stat), f_stat(_f_stat) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        std::string tabs;
	for (int i = 0; i < SCP; i++) tabs += "\t";
	stream << tabs;
        stream << "if(";
        cond->PYTHON_t(stream, SCP);
        stream << "):\n";
        SCP++;
        t_stat->PYTHON_t(stream, SCP);
        SCP--;
	for (int i = 0; i < SCP; i++) tabs += "\t";
	stream << tabs;
        stream << "else:\n";
        SCP++;
        f_stat->PYTHON_t(stream, SCP);
        SCP--;
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        std::string LabelElse = CONT.n_LAB("ELSE");
        std::string LabelEnd = CONT.n_LAB("END");
        cond->C90_COMP(stream, CONT);
        stream << "\tbeq $" << CONT.R2 << ",$"
               << "0,$" << LabelElse << "\n";
        stream << "\tnop\n";
        t_stat->C90_COMP(stream, CONT);
        stream << "\tb $" << LabelEnd << "\n";
        stream << "\tnop\n";
        stream << "$" << LabelElse << ":\n";
        f_stat->C90_COMP(stream, CONT);
        stream << "$" << LabelEnd << ":\n";
    }
};

class While : public LEAF
{
private:
    const Pointer_LEAF cond;
    const Pointer_LEAF block;

public:
    While(const Pointer_LEAF _cond, const Pointer_LEAF _block) : cond(_cond), block(_block) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        std::string tabs;
	for (int i = 0; i < SCP; i++) tabs += "\t";
	stream << tabs;
        stream << "while(";
        cond->PYTHON_t(stream, SCP);
        stream << "): \n";
        SCP++;
        block->PYTHON_t(stream, SCP);
        SCP--;
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        std::string blockLabel = CONT.n_LAB("BODY");
        std::string condLabel = CONT.n_LAB("COND");
        stream << "\tb $" << condLabel << "\n";
        stream << "\tnop\n";
        stream << "$" << blockLabel << ":\n";
        block->C90_COMP(stream, CONT);
        stream << "$" << condLabel << ":\n";
        cond->C90_COMP(stream, CONT);
        stream << "\tbne $" << CONT.R2 << ",$0,$" << blockLabel << "\n";
        stream << "\tnop\n";
    }
};

class DoWhile : public LEAF
{
private:
    const Pointer_LEAF cond;
    const Pointer_LEAF block;

public:
    DoWhile(const Pointer_LEAF _cond, const Pointer_LEAF _block) : cond(_cond), block(_block) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "DoWhile not in python spec\n";
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        std::string blockLabel = CONT.n_LAB("BODY");
        std::string condLabel = CONT.n_LAB("COND");
        stream << "$" << blockLabel << ":\n";
        block->C90_COMP(stream, CONT);
        stream << "$" << condLabel << ":\n";
        cond->C90_COMP(stream, CONT);
        stream << "\tbne $" << CONT.R2 << ",$0,$" << blockLabel << "\n";
        stream << "\tnop\n";
    }
};

class For : public LEAF
{
private:
    const Pointer_LEAF initial;
    const Pointer_LEAF cond;
    const Pointer_LEAF step;
    const Pointer_LEAF block;

public:
    For(const Pointer_LEAF _initial, const Pointer_LEAF _cond, const Pointer_LEAF _step, const Pointer_LEAF _block) : initial(_initial), cond(_cond), step(_step), block(_block) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "For not in python spec\n";
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        std::string blockLabel = CONT.n_LAB("BODY");
        std::string condLabel = CONT.n_LAB("COND");
        initial->C90_COMP(stream, CONT);
        stream << "\tb $" << condLabel << "\n";
        stream << "\tnop\n";
        stream << "$" << blockLabel << ":\n";
        block->C90_COMP(stream, CONT);
        step->C90_COMP(stream, CONT);
        stream << "$" << condLabel << ":\n";
        cond->C90_COMP(stream, CONT);
        stream << "\tbne $" << CONT.R2 << ",$0,$" << blockLabel << "\n";
        stream << "\tnop\n";
    }
};

class Return : public LEAF
{
private:
    const Pointer_LEAF exp;

public:
    Return(const Pointer_LEAF _exp) : exp(_exp) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        std::string tabs;
	for (int i = 0; i < SCP; i++) tabs += "\t";
	stream << tabs;
        stream << "return ";
        if (exp != NULL)
        {
            exp->PYTHON_t(stream, SCP);
        }
        stream << "\n";
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        if (exp != NULL)
        {
            exp->C90_COMP(stream, CONT);
        }
        else
        {
            stream << "\tmove $2,$" << CONT.R1 << "\n";
            stream << "\t"
                   << "move $sp,$fp\n";
            stream << "\tlw $31," << CONT.siz_fra - 4 << "($sp)\n";
            stream << "\tnop\n";
            stream << "\t"
                   << "lw $fp," << CONT.siz_fra - 8 << "($sp)\n";
            stream << "nop\n";
            stream << "\t"
                   << "addiu $sp,$sp," << CONT.siz_fra << "\n";
            stream << "\tj $31\n";
            stream << "\tnop\n";
        }
    }
};

class Seq : public LEAF
{
private:
    const Pointer_LEAF sequ;
    const Pointer_LEAF state;

public:
    Seq(const Pointer_LEAF _sequ, const Pointer_LEAF _state) : sequ(_sequ), state(_state) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        if (sequ != NULL)
        {
            sequ->PYTHON_t(stream, SCP);
        }
        state->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        if (sequ != NULL)
        {
            sequ->C90_COMP(stream, CONT);
        }
        state->C90_COMP(stream, CONT);
    }
};

#endif
