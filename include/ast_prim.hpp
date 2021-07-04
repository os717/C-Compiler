#ifndef AST_PRIM_HPP
#define AST_PRIM_HPP

#include <iostream>
#include <string>
#include <vector>
#include "CONT.hpp"
#include "ast_leaf.hpp"

class LEAF;
typedef const LEAF *Pointer_LEAF;

class Expr_List : public LEAF
{
private:
    const Pointer_LEAF exp_list;
    const Pointer_LEAF exp;

public:
    Expr_List(const Pointer_LEAF _exp_list, const Pointer_LEAF _exp) : exp_list(_exp_list), exp(_exp) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        if (exp_list != NULL)
        {
            exp_list->PYTHON_t(stream, SCP);
            stream << "\n";
        }
        exp->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        if (exp_list != NULL)
        {
            exp_list->C90_COMP(stream, CONT);
            stream << "\n";
        }
        exp->C90_COMP(stream, CONT);
    }
};

class Ass_Equ : public LEAF
{
private:
    const Pointer_LEAF v_left;
    const Pointer_LEAF v_right;

public:
    Ass_Equ(const Pointer_LEAF _v_left, const Pointer_LEAF _v_right) : v_left(_v_left), v_right(_v_right) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        std::string tabs;
	for (int i = 0; i < SCP; i++) tabs += "\t";
	stream << tabs;
        v_left->PYTHON_t(stream, SCP);
        stream << "=";
        v_right->PYTHON_t(stream, SCP);
        stream << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        std::string NAM = v_left->fetchnam();
        CONT.R1 = 2;
        CONT.R2 = 2;
        if (CONT.num_sco == 0)
        {
            stream << "\t.globl " << NAM << "\n";
            stream << "\t.data\n";
            stream << "\t.align 2\n";
            stream << "\t.type a,@object\n";
            stream << "\t.size " << NAM << ",4\n";
            stream << NAM << ":\n";
            stream << "\t.word ";
            v_right->C90_COMP(stream, CONT);
            stream << "\n";
        }
        else if (CONT.GELS[CONT.num_sco].count(NAM) > 0)
        {
            v_right->C90_COMP(stream, CONT);
            stream << "\tsw $2," << CONT.siz_fra - CONT.OfsFin(NAM) << "($fp)\n";
        }
        else
        {
            v_right->C90_COMP(stream, CONT);
            CONT.siz_fra += 4;
            stream << "\n\taddiu $sp,$sp,-4\n";
            stream << "\tmove $fp,$sp\n";
            CONT.GELS[CONT.num_sco][NAM] = CONT.siz_fra;
            stream << "\tsw $2," << CONT.siz_fra - CONT.OfsFin(NAM) << "($fp)\n";
        }
    }
};

class Tern_If : public LEAF
{
private:
    const Pointer_LEAF cond;
    const Pointer_LEAF t_value;
    const Pointer_LEAF f_value;

public:
    Tern_If(const Pointer_LEAF _cond, const Pointer_LEAF _t_value, const Pointer_LEAF _f_value) : cond(_cond), t_value(_t_value), f_value(_f_value) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "Tern_If not in python spec\n";
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        std::string LabelFalse = CONT.n_LAB("FALSE");
        std::string LabelEnd = CONT.n_LAB("END");
        cond->C90_COMP(stream, CONT);
        int dReg = CONT.R2;
        stream << "\n\tbeq $" << dReg << ",$0,$" << LabelFalse << "\n";
        t_value->C90_COMP(stream, CONT);
        stream << "\tb $" << LabelEnd << "\n";
        stream << "$" << LabelFalse << ":\n";
        f_value->C90_COMP(stream, CONT);
        stream << "$" << LabelEnd << ":\n";
    }
};

class Val : public LEAF
{
private:
    const double *Int;

public:
    Val(const double *_Int) : Int(_Int) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << *Int;
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        if (CONT.num_sco == 0)
        {
            stream << *Int;
        }
        else
        {
            stream << "\tli $" << CONT.R2 << "," << *Int << "\n";
        }
    }

    std::string fetchnam() const
    {
        return std::to_string(*Int);
    }
};

class Var : public LEAF
{
private:
    const std::string *vari;

public:
    Var(const std::string *_vari) : vari(_vari) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << *vari;
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        if (CONT.gl_CHK(*vari))
        {
            stream << "\tlui $" << CONT.R2 << ",%hi(" << *vari << ")\n";
            stream << "\tlw $" << CONT.R2 << ",%lo(" << *vari << ")($" << CONT.R2 << ")\n";
            stream << "\tnop\n";
        }
        else
        {
            stream << "\tlw $" << CONT.R2 << "," << CONT.siz_fra - CONT.OfsFin(*vari) << "($fp)"
                   << "\n";
            stream << "\tnop\n";
        }
    }

    std::string fetchnam() const
    {
        return *vari;
    }
};

class Call_Func : public LEAF
{
public:
    Call_Func(const std::string *_NAM_fun, const Pointer_LEAF _id) : NAM_fun(_NAM_fun), id(_id) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << *NAM_fun;
        stream << "(";
        if (id != NULL)
        {
            id->PYTHON_t(stream, SCP);
        }
        stream << ")";
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        stream << "# Call_Func not implemented \n";
    }

private:
    const std::string *NAM_fun;
    const Pointer_LEAF id;
};
class Brac_Exp : public LEAF
{
private:
    const Pointer_LEAF exp;

public:
    Brac_Exp(const Pointer_LEAF _exp) : exp(_exp) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "(";
        exp->PYTHON_t(stream, SCP);
        stream << ")";
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        exp->C90_COMP(stream, CONT);
    }
};

#endif
