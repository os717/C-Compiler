#ifndef AST_DEC_HPP
#define AST_DEC_HPP

#include <iostream>
#include <string>
#include <vector>
#include "cont.hpp"
#include "ast_leaf.hpp"

class LEAF;
typedef const LEAF *Pointer_LEAF;

class Dec_List : public LEAF
{
private:
    const Pointer_LEAF dec, list;

public:
    Dec_List(const Pointer_LEAF _list, const Pointer_LEAF _dec) : list(_list), dec(_dec) {}
    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        if (list != NULL)
        {
            list->PYTHON_t(stream, SCP);
            stream << '\n';
        }
        dec->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        if (list != NULL)
        {
            list->C90_COMP(stream, CONT);
        }
        dec->C90_COMP(stream, CONT);
    }
};

class Def_Func : public LEAF
{
private:
    const std::string *type, *NAM;
    const Pointer_LEAF pars, block;

public:
    Def_Func(const std::string *_type, const std::string *_NAM, const Pointer_LEAF _pars, const Pointer_LEAF _block) : type(_type), NAM(_NAM), pars(_pars), block(_block) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << "def " << *NAM << "(";
        if (pars != NULL)
        {
            pars->PYTHON_t(stream, SCP);
        }
        stream << "):\n";
        for (int i = 0; i < variables_global.size(); i++)
        {
            stream << "\tglobal " << variables_global[i] << '\n';
        }
        SCP++;
        block->PYTHON_t(stream, SCP);
        SCP--;
        stream << '\n';
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        stream << "\t.text\n";
        stream << "\t.align 2\n";
        std::string label = *NAM;
        stream << "\t.global " << label << "\n";
        stream << "\t.set nomicromips\n";
        stream << "\t.set nomips16\n";
        stream << "\t.type " << label << ", @function\n";
        stream << label << ":\n";
        stream << "\t"
               << "addiu $sp,$sp,-" << CONT.siz_fra << "\n";
        stream << "\tsw $31," << CONT.siz_fra - 4 << "($sp)\n";
        stream << "\tsw $fp," << CONT.siz_fra - 8 << "($sp)\n";
        stream << "\t"
               << "move $fp,$sp\n";
        CONT.num_paramet = 0;
        CONT.num_sco++;
        if (pars != NULL)
        {
            pars->C90_COMP(stream, CONT);
        }
        CONT.num_paramet = 0;
        stream << "\n";
        CONT.R1 = 2;
        CONT.R2 = 2;
        block->C90_COMP(stream, CONT);
        stream << "\n";
        CONT.num_sco--;

        stream << "\t"
               << "move $sp,$fp\n";
        stream << "\tlw $31," << CONT.siz_fra - 4 << "($sp)\n";
        stream << "\tnop\n";
        stream << "\t"
               << "lw $fp," << CONT.siz_fra - 8 << "($sp)\n";
        stream << "\t"
               << "addiu $sp,$sp," << CONT.siz_fra << "\n";
        stream << "\t"
               << "j $31\n";
        stream << "\t"
               << "nop\n";
    }
};

class List_Para : public LEAF
{
private:
    const Pointer_LEAF par_list, parm;

public:
    List_Para(const Pointer_LEAF _par_list, const Pointer_LEAF _parm) : par_list(_par_list), parm(_parm) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        if (par_list != NULL)
        {
            par_list->PYTHON_t(stream, SCP);
            stream << ", ";
        }
        parm->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        if (par_list != NULL)
        {
            par_list->C90_COMP(stream, CONT);
        }
        parm->C90_COMP(stream, CONT);
    }
};

class Dec_Para : public LEAF
{
private:
    const std::string *NAM, *type;

public:
    Dec_Para(const std::string *_type, const std::string *_NAM) : type(_type), NAM(_NAM) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        stream << *NAM;
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        CONT.num_paramet++;
        int offset = CONT.siz_fra + 4 * (CONT.num_paramet - 1);
        int reg = CONT.num_paramet + 3;
        stream << "\tsw $" << reg << "," << offset << "($fp)\n";
        CONT.GELS[CONT.num_sco][*NAM] = CONT.siz_fra - offset;
    }
};

class Dec_Norm : public LEAF
{
private:
    const std::string *modi;
    const std::string *type;
    const Pointer_LEAF var_list;

public:
    Dec_Norm(const std::string *_modi, const std::string *_type, const Pointer_LEAF _var_list) : modi(_modi), type(_type), var_list(_var_list) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        var_list->PYTHON_t(stream, SCP);
        stream << "\n";
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        var_list->C90_COMP(stream, CONT);
    }
};

class Var_List : public LEAF
{
private:
    const Pointer_LEAF var_list;
    const Pointer_LEAF var_dec;

public:
    Var_List(const Pointer_LEAF _var_list, const Pointer_LEAF _var_dec) : var_list(_var_list), var_dec(_var_dec) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        if (var_list != NULL)
        {
            var_list->PYTHON_t(stream, SCP);
            stream << "\n";
        }
        var_dec->PYTHON_t(stream, SCP);
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        if (var_list != NULL)
        {
            var_list->C90_COMP(stream, CONT);
            stream << "\n";
        }
        var_dec->C90_COMP(stream, CONT);
    }
};

class Dec_Var : public LEAF
{
private:
    const std::string *NAM;
    const Pointer_LEAF value;

public:
    Dec_Var(const std::string *_NAM, const Pointer_LEAF _value) : NAM(_NAM), value(_value) {}

    void PYTHON_t(std::ostream &stream, int &SCP) const
    {
        if (SCP == 0)
        {
            variables_global.push_back(*NAM);
        }
        else
        {
            std::string tabs;
	for (int i = 0; i < SCP; i++) tabs += "\t";
	stream << tabs;
        }
        stream << *NAM << "=";
        if (value != NULL)
        {
            value->PYTHON_t(stream, SCP);
        }
        else
        {
            stream << "0";
        }
    }

    void C90_COMP(std::ostream &stream, CONT &CONT) const
    {
        CONT.R1 = 2;
        CONT.R2 = 2;
        if (CONT.num_sco == 0)
        {
            stream << "\t.globl " << *NAM << "\n";
            stream << "\t.data\n";
            stream << "\t.align 2\n";
            stream << "\t.type a,@object\n";
            stream << "\t.size " << *NAM << ",4\n";
            stream << *NAM << ":\n";
            stream << "\t.word ";
            value->C90_COMP(stream, CONT);
            stream << "\n";
        }
        else if (CONT.GELS[CONT.num_sco].count(*NAM) > 0)
        {
            value->C90_COMP(stream, CONT);
            stream << "\tsw $2," << CONT.siz_fra - CONT.OfsFin(*NAM) << "($fp)\n";
        }
        else
        {
            value->C90_COMP(stream, CONT);
            CONT.siz_fra += 4;
            stream << "\taddiu $sp,$sp,-4\n";
            stream << "\tmove $fp,$sp\n";
            CONT.GELS[CONT.num_sco][*NAM] = CONT.siz_fra;
            stream << "\tsw $2," << CONT.siz_fra - CONT.OfsFin(*NAM) << "($fp)\n";
        }
    }
};

#endif
