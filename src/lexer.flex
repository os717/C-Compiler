%option noyywrap
%option yylineno

%{
  #include "parser.tab.hpp"
  #include <string>
  extern "C" int fileno(FILE *stream);
%}

ID  [_a-zA-Z]+[_a-zA-Z0-9]*
DEC_DIG [0-9]
HEX_DIG [{DEC_DIG}a-fA-F]
OCT_DIG [0-7]
SUF ([u|U][l|L]|[l|L][u|U])[l|L]?
DEC_VAL {DEC_DIG}+{SUF}?
HEX_VAL [0x|0X]{HEX_DIG}+{SUF}?
OCT_VAL 0{OCT_DIG}+{SUF}?

%%

"char"					{return CHAR_TYPE;}
"double"				{return DOUB_TYPE;}
"enum"			  	{return ENUM_TYPE;}
"float"					{return FLOAT_TYPE;}
"int"						{return INT_TYPE;}
"long"					{return LONG_TYPE;}
"short"					{return SHORT_TYPE;}
"signed"				{return SIGNED_TYPE;}
"struct"				{return STRUCT_TYPE;}
"union"					{return UNION_TYPE;}
"unsigned"			{return UNSIGNED_TYPE;}
"void"					{return VOID_TYPE;}
"auto"					{return AUTO_TYPE;}
"const"         {return CONST_TYPE;}
"extern"				{return EXTERN_TYPE;}
"register"			{return REG_TYPE;}
"sizeof"        {return SIZE_TYPE;}
"static"				{return STATIC_TYPE;}
"typedef"       {return TDEF_TYPE;}
"volatile"      {return VOLA_TYPE;}
"break"         {return BREAK_TYPE;}
"case"          {return CASE_TYPE;}
"continue"      {return CONT_TYPE;}
"default"       {return DEFAULT_TYPE;}
"do"            {return DO_TYPE;}
"else"          {return ELSE_TYPE;}
"for"           {return FOR_TYPE;}
"if"            {return IF_TYPE;}
"return"        {return RET_TYPE;}
"switch"        {return SWCH_TYPE;}
"while"         {return WHILE_TYPE;}
"=="		  			{return EQU_TYPE;}
"!="		  			{return NEQU_TYPE;}
"<"			  			{return LESS_TYPE;}
">"			  			{return GREAT_TYPE;}
"<="						{return LESSEQU_TYPE;}
">="						{return GREATEQU_TYPE;}
"&&"						{return LOGAND_TYPE;}
"||"						{return LOGOR_TYPE;}
"!"					  	{return LOGNOT_TYPE;}
"+"					   	{return PLUS_TYPE;}
"-"					   	{return MINUS_TYPE;}
"*"					    {return MUL_TYPE;}
"/"					    {return DIV_TYPE;}
"%"					    {return MOD_TYPE;}
"+="					  {return PLUSEQU_TYPE;}
"-="			      {return MINUSEQU_TYPE;}
"*="						{return MULEQU_TYPE;}
"/="					  {return DIVEQU_TYPE;}
"%="  					{return MODEQU_TYPE;}
"++"	  				{return INCR_TYPE;}
"--"		  			{return DECR_TYPE;}
"&"							{return BITAND_TYPE;}
"|"							{return BITOR_TYPE;}
"^"							{return BITXOR_TYPE;}
"~"							{return BITNOT_TYPE;}
"<<"						{return SHIFTL_TYPE;}
">>"						{return SHIFTR_TYPE;}
"&="            {return ANDEQU_TYPE;}
"|="            {return OREQU_TYPE;}
"^="            {return XOREQU_TYPE;}
"<<="						{return SHIFTLEQU_TYPE;}
">>="						{return SHIFTREQU_TYPE;}
"{"						  {return CURLBRACL_TYPE;}
"}"						  {return CURLBRACR_TYPE;}
"("			        {return BRACL_TYPE;}
")"						  {return BRACR_TYPE;}
"["				      {return SQRBRACL_TYPE;}
"]"			       	{return SQRBRACR_TYPE;}
"->"				  	{return ARROW_TYPE;}
":"						  {return COLON_TYPE;}
","			        {return COMMA_TYPE;}
"##"				    {return DOUBHASH_TYPE;}
"#"			       	{return HASH_TYPE;}
"."			       	{return PERIOD_TYPE;}
";"			       	{return SCOLON_TYPE;}
"?"			       	{return TERNOP_TYPE;}
"..."			      {return ELLIP_TYPE;}
"="			        {return ASSIGN_TYPE;}
{ID}		{
                  yylval.str=new std::string(yytext);
                  return IDENT_TYPE;
                }
{DEC_VAL}				{
                  yylval.number= new double(strtol(yytext, NULL, 10));
                  return INTEGER_TYPE;
                }
{HEX_VAL}						{
                  yylval.number = new double(strtol(yytext,NULL,16));
                  return INTEGER_TYPE;
                }
{OCT_VAL}					{
                  yylval.number = new double(strtol(yytext,NULL,8));
                  return INTEGER_TYPE;
                }
"//".*		          {;}
[ \t\r\n]+ 				  {;}
<<EOF>> {yyterminate();}
%%
void yyerror (char const *s){
  fprintf (stderr, "Flex Error: %s\n", s);
  exit(1);
}
