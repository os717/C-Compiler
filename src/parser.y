%code requires{
	#include "../include/ast_leaf.hpp"
  #include "../include/ast_arith.hpp"
  #include "../include/ast_dec.hpp"
  #include "../include/ast_logic.hpp"
  #include "../include/ast_prim.hpp"
  #include "../include/ast_state.hpp"
	#include <cassert>
  #include <fstream>
	extern Pointer_LEAF root;
	int yylex(void);
	void yyerror(const char*);
	extern FILE* yyin;
}
%union{
	const LEAF *node;
	std::string *str;
	double *number;
}
%token CHAR_TYPE DOUB_TYPE ENUM_TYPE FLOAT_TYPE INT_TYPE LONG_TYPE SHORT_TYPE SIGNED_TYPE STRUCT_TYPE UNION_TYPE UNSIGNED_TYPE VOID_TYPE
%token AUTO_TYPE CONST_TYPE EXTERN_TYPE REG_TYPE SIZE_TYPE STATIC_TYPE TDEF_TYPE VOLA_TYPE
%token BREAK_TYPE CASE_TYPE CONT_TYPE DEFAULT_TYPE DO_TYPE ELSE_TYPE FOR_TYPE IF_TYPE RET_TYPE SWCH_TYPE WHILE_TYPE
%token EQU_TYPE NEQU_TYPE LESS_TYPE GREAT_TYPE LESSEQU_TYPE GREATEQU_TYPE LOGAND_TYPE LOGOR_TYPE LOGNOT_TYPE
%token PLUS_TYPE MINUS_TYPE MUL_TYPE DIV_TYPE MOD_TYPE PLUSEQU_TYPE MINUSEQU_TYPE MULEQU_TYPE DIVEQU_TYPE MODEQU_TYPE INCR_TYPE DECR_TYPE
%token BITAND_TYPE BITOR_TYPE BITXOR_TYPE BITNOT_TYPE SHIFTL_TYPE SHIFTR_TYPE ANDEQU_TYPE OREQU_TYPE XOREQU_TYPE SHIFTLEQU_TYPE SHIFTREQU_TYPE
%token CURLBRACL_TYPE CURLBRACR_TYPE BRACL_TYPE BRACR_TYPE SQRBRACL_TYPE SQRBRACR_TYPE
%token ARROW_TYPE COLON_TYPE COMMA_TYPE DOUBHASH_TYPE HASH_TYPE PERIOD_TYPE SCOLON_TYPE TERNOP_TYPE ELLIP_TYPE ASSIGN_TYPE
%token IDENT_TYPE INTEGER_TYPE

%type<node> ROOT Declares
%type<node> Declare
%type<node> Dec_Func
%type<node> List_Para
%type<node> Para
%type<node> Dec_Var_List
%type<node> Dec_Var
%type<node> Var_List

%type<str> Modi
%type<str> Type
%type<str> Sign
%type<str> Scal

%type<node> State
%type<node> Then_State
%type<node> Normal_State
%type<node> Body
%type<node> Seq
%type<node> Expr
%type<node> Ass_Op
%type<node> If_Op
%type<node> Or_Op
%type<node> And_Op
%type<node> Bit_Or_Op
%type<node> Bit_Xor_Op
%type<node> Bit_And_Op
%type<node> Equate_Op
%type<node> Comp_Op
%type<node> Shift_Op
%type<node> Add_Op
%type<node> Mul_Op
%type<node> Unary_Op
%type<node> Val
%type<node> Id_List

%type<str> IDENT_TYPE

%type<number> INTEGER_TYPE

%type<str> Argument 

%nonassoc "else"

%start ROOT

%%

ROOT:	Declares 	{ root = $1; }

Declares : Declares Declare {$$ = new Dec_List($1, $2);}
             | Declare {$$ = new Dec_List(NULL, $1);}
             ;

Declare : Dec_Func {$$ = $1;}
            | Dec_Var_List {$$ = $1;}
            ;

Dec_Func : Type IDENT_TYPE BRACL_TYPE List_Para  BRACR_TYPE Body {$$ = new Def_Func($1, $2, $4, $6);}
                    | IDENT_TYPE BRACL_TYPE List_Para  BRACR_TYPE Body {$$ = new Def_Func(new std::string("int"), $1, $3, $5);}
                    | Type IDENT_TYPE BRACL_TYPE BRACR_TYPE Body {$$ = new Def_Func($1, $2, NULL, $5);}
                    | IDENT_TYPE BRACL_TYPE BRACR_TYPE Body {$$ = new Def_Func(new std::string("int"), $1, NULL, $4);}
                    ;

List_Para : List_Para COMMA_TYPE Para{$$ = new List_Para($1, $3);}
           		| Para {$$ = new List_Para(NULL, $1);}
           		;

Para: CONST_TYPE Type IDENT_TYPE {$$ = new Dec_Para($2, $3);}
          | Type IDENT_TYPE {$$ = new Dec_Para($1, $2);}
          | Type {$$ = new Dec_Para($1, NULL);}
          ;

Dec_Var_List : Modi Type Var_List  SCOLON_TYPE {$$ = new Dec_Norm($1, $2, $3);}
                    		| Type Var_List  SCOLON_TYPE {$$ = new Dec_Norm(NULL, $1, $2);}
                    		| Modi Var_List  SCOLON_TYPE {$$ = new Dec_Norm($1, NULL, $2);}
                    		;

Var_List : Var_List COMMA_TYPE Dec_Var {$$ = new Var_List($1, $3);}
             | Dec_Var {$$ = new Var_List(NULL, $1);}
             ;

Dec_Var : IDENT_TYPE ASSIGN_TYPE Or_Op {$$ = new Dec_Var($1, $3);}
										| IDENT_TYPE {$$ = new Dec_Var($1, NULL);}
										;

Modi : EXTERN_TYPE {$$ = new std::string("extern");}
         | STATIC_TYPE {$$ = new std::string("static");}
         | REG_TYPE {$$ = new std::string("register");}
         | AUTO_TYPE {$$ = new std::string("auto");}
         | VOLA_TYPE {$$ = new std::string("volatile");}
         | CONST_TYPE {$$ = new std::string("const");}
         ;

Type : Sign Scal {$$ = new std::string(*$1 + " " + *$2);}
     | Scal {$$ = $1;}

Sign : SIGNED_TYPE {$$ = new std::string("signed");}
     | UNSIGNED_TYPE {$$ = new std::string("unsigned");}
     ;

Scal : CHAR_TYPE {$$ = new std::string("char");}
       | INT_TYPE {$$ = new std::string("int");}
       | SHORT_TYPE {$$ = new std::string("short");}
       | LONG_TYPE {$$ = new std::string("long");}
       | SHORT_TYPE INT_TYPE {$$ = new std::string("short int");}
       | LONG_TYPE INT_TYPE {$$ = new std::string("long int");}
       | VOID_TYPE {$$ = new std::string("void");}
       ;

State : Dec_Var_List  {$$ = $1;}
          | IF_TYPE BRACL_TYPE Expr BRACR_TYPE State {$$ = new If($3, $5);}
          | IF_TYPE BRACL_TYPE Expr BRACR_TYPE Then_State ELSE_TYPE State {$$ = new If_Else($3, $5, $7);}
					| WHILE_TYPE BRACL_TYPE Expr BRACR_TYPE State {$$ = new While($3, $5);}
					| FOR_TYPE BRACL_TYPE Expr SCOLON_TYPE Expr SCOLON_TYPE Expr BRACR_TYPE State {$$ = new For($3, $5, $7, $9);}
					| FOR_TYPE BRACL_TYPE Dec_Var_List Expr SCOLON_TYPE Expr BRACR_TYPE State {$$ = new For($3, $4, $6, $8);}
			    | Normal_State {$$ = $1;}
          ;

Then_State : IF_TYPE BRACL_TYPE Expr BRACR_TYPE Then_State ELSE_TYPE Then_State {$$ = new If_Else($3, $5, $7);}
              | WHILE_TYPE BRACL_TYPE Expr BRACR_TYPE Then_State  {$$ = new While($3, $5);}
              | Normal_State {$$ = $1;}
              ;

Normal_State : Body {$$ = $1;}
								| DO_TYPE State WHILE_TYPE BRACL_TYPE Expr BRACR_TYPE SCOLON_TYPE {$$ = new DoWhile($5, $2);}
								| Expr SCOLON_TYPE {$$ = new Expr_List(NULL, $1);}
                | RET_TYPE Expr SCOLON_TYPE {$$ = new Return($2);}
                | RET_TYPE SCOLON_TYPE {$$ = new Return(NULL);}
                | SCOLON_TYPE {}
                ;

Body : CURLBRACL_TYPE Seq CURLBRACR_TYPE {$$ = $2;}
      ;

Seq : Seq State {$$ = new Seq($1, $2);}
         | State {$$ = new Seq(NULL, $1);}
         ;

Expr : Expr COMMA_TYPE Ass_Op {$$ = new Expr_List($3, $1);}
           | Ass_Op {$$ = new Expr_List(NULL, $1);}
           ;

Ass_Op : Ass_Op ASSIGN_TYPE If_Op {$$ = new Ass_Equ($1, $3);}
								| Ass_Op PLUSEQU_TYPE If_Op {$$ = new Ass_Equ($1, new Add($1, $3));}
								| Ass_Op MINUSEQU_TYPE If_Op {$$ = new Ass_Equ($1, new Sub($1, $3));}
								| Ass_Op MULEQU_TYPE If_Op {$$ = new Ass_Equ($1, new Mul($1, $3));}
								| Ass_Op DIVEQU_TYPE If_Op {$$ = new Ass_Equ($1, new Div($1, $3));}
								| Ass_Op MODEQU_TYPE If_Op {$$ = new Ass_Equ($1, new Mod($1, $3));}
								| Ass_Op OREQU_TYPE If_Op {$$ = new Ass_Equ($1, new Bit_Or($1, $3));}
								| Ass_Op XOREQU_TYPE If_Op {$$ = new Ass_Equ($1, new Bit_Xor($1, $3));}
								| Ass_Op ANDEQU_TYPE If_Op {$$ = new Ass_Equ($1, new Bit_And($1, $3));}
								| Ass_Op SHIFTLEQU_TYPE If_Op {$$ = new Ass_Equ($1, new Shift_L($1, $3));}
								| Ass_Op SHIFTREQU_TYPE If_Op {$$ = new Ass_Equ($1, new Shift_R($1, $3));}
								| If_Op {$$ = $1;}
                ;

If_Op : Or_Op TERNOP_TYPE If_Op COLON_TYPE If_Op {$$ = new Tern_If($1, $3, $5);}
            | Or_Op {$$ = $1;}
            ;

Or_Op : Or_Op LOGOR_TYPE And_Op {$$ = new Log_Or($1, $3);}
            | And_Op {$$ = $1;}
            ;

And_Op : And_Op LOGAND_TYPE Bit_Or_Op {$$ = new Log_And($1, $3);}
             | Bit_Or_Op {$$ = $1;}
             ;

Bit_Or_Op : Bit_Or_Op BITOR_TYPE Bit_Xor_Op {$$ = new Bit_Or($1, $3);}
            			 | Bit_Xor_Op {$$ = $1;}
            			 ;

Bit_Xor_Op : Bit_Xor_Op BITXOR_TYPE Bit_And_Op {$$ = new Bit_Xor($1, $3);}
			              | Bit_And_Op {$$ = $1;}
			              ;

Bit_And_Op : Bit_And_Op BITAND_TYPE Equate_Op {$$ = new Bit_And($1, $3);}
          					| Equate_Op {$$ = $1;}
          					;

Equate_Op : Equate_Op EQU_TYPE Comp_Op {$$ = new Equ($1, $3);}
								| Equate_Op NEQU_TYPE Comp_Op {$$ = new N_Equ($1, $3);}
                | Comp_Op {$$ = $1;}
                ;

Comp_Op : Comp_Op LESS_TYPE Add_Op {$$ = new Less($1, $3);}
                 | Comp_Op GREAT_TYPE Add_Op {$$ = new Great($1, $3);}
								 | Comp_Op LESSEQU_TYPE Add_Op {$$ = new Less_Equ($1, $3);}
								 | Comp_Op GREATEQU_TYPE Add_Op {$$ = new Great_Equ($1, $3);}
								 | Shift_Op {$$ = $1;}
                 ;

Shift_Op : Shift_Op SHIFTL_TYPE Add_Op {$$ = new Shift_L($1, $3);}
            	 | Shift_Op SHIFTR_TYPE Add_Op {$$ = new Shift_R($1, $3);}
            	 | Add_Op {$$ = $1;}
            	 ;

Add_Op : Add_Op PLUS_TYPE Mul_Op {$$ = new Add($1, $3);}
             | Add_Op MINUS_TYPE Mul_Op {$$ = new Sub($1, $3);}
             | Mul_Op {$$ = $1;}
             ;

Mul_Op : Mul_Op MUL_TYPE Unary_Op {$$ = new Mul($1, $3);}
									| Mul_Op DIV_TYPE Unary_Op {$$ = new Div($1, $3);}
									| Mul_Op MOD_TYPE Unary_Op {$$ = new Mod($1, $3);}
									| Unary_Op {$$ = $1;}
                  ;

Unary_Op : MINUS_TYPE Unary_Op {$$ = new Unary_Minus($2);}
							 | BITNOT_TYPE Unary_Op {$$ = new Bit_Not($2);}
 							 | LOGNOT_TYPE Unary_Op {$$ = new Log_Not($2);}
							 | INCR_TYPE Unary_Op {$$ = new Incr_Pre($2);}
							 | DECR_TYPE Unary_Op {$$ = new Decr_Pre($2);}
							 | Unary_Op INCR_TYPE {$$ = new Incr_Post($1);}
							 | Unary_Op DECR_TYPE {$$ = new Decr_Post($1);}
               | Val {$$ = $1;}
               ;

Val : INTEGER_TYPE {$$ = new Val($1);}
      | IDENT_TYPE BRACL_TYPE Id_List BRACR_TYPE {$$ = new Call_Func($1, $3);}
      | IDENT_TYPE BRACL_TYPE BRACR_TYPE {$$ = new Call_Func($1, NULL);}
      | IDENT_TYPE {$$ = new Var($1);}
      | BRACL_TYPE Expr BRACR_TYPE {$$ = new Brac_Exp($2);}
      ;

Id_List : Id_List COMMA_TYPE Argument {$$ = new List_Para($1, new Dec_Para(NULL, $3));}
       | Argument {$$ = new List_Para(NULL, new Dec_Para(NULL, $1));}
       ;

Argument : IDENT_TYPE {$$ = $1;}
				 | INTEGER_TYPE {$$ = new std::string(std::to_string((int)*$1));}
				 ;

%%

Pointer_LEAF root;

const Pointer_LEAF DO_PARSE(const char file[]){
	root=NULL;
	yyin = fopen(file, "r");
	yyparse();
	return root;
}
