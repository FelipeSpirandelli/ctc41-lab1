/****************************************************/
/* File: tiny.y                                     */
/* The TINY Yacc/Bison specification file           */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static char * savedName; /* for use in assignments */
static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void);
int yyerror(char *);

%}

%union {
  int val;
  char *name;
  TokenType token;
  TreeNode* node;
  ExpType type;
}

/* Tokens sem valores */
%token PLUS MINUS TIMES OVER LT LTE GT GTE EQ ASSIGN DIFF SEMI COMMA LPAREN RPAREN LSBRAC RSBRAC LCBRAC RCBRAC
%token IF ELSE WHILE
%token VOID INT RETURN
%token ERROR

/* Tokens com valores*/
%token <val> NUM
%token <name> ID

/* Símbolos não terminais */ 
%type <node> programa declaracao_lista declaracao var_declaracao fun_declaracao params param_lista param composto_decl local_declaracoes statement_lista statement expressao_decl selecao_decl iteracao_decl retorno_decl expressao var simples_expressao soma_expressao termo fator ativacao args arg_lista
%type <type> tipo_especificador
%type <token> soma mult relacional
 
/* utilidades */
%start programa
%left PLUS MINUS
%right TIMES OVER ASSIGN
%nonassoc LT LTE GT GTE EQ DIFF
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%% /* Grammar for CMINUS */

programa     : declaracao_lista
                 { savedTree = $1;} 
            ;
declaracao_lista :  declaracao_lista declaracao
                 {
                    TreeNode* t = $1;
                    if (t != NULL) {
                      while (t->sibling != NULL) t = t->sibling;
                      t->sibling = $2;
                      $$ = $1;
                    }
                    else $$ = $2;

                 }
            | declaracao
                 {
                    $$ = $1;
                 }
            ;
declaracao  : var_declaracao 
                 {
                    $$ = $1;
                 }
            | fun_declaracao 
                {
                    $$ = $1;
                }
            ;
var_declaracao : tipo_especificador ID SEMI 
                {
                    $$ = newDeclNode(VarK);
                    $$->type = $1;
                    $$->attr.name = $2;
                    // $$->attr.val = 1; // valor padrao para variaveis que não são arrays. O valor será utilizado para diferenciar arrays de não arrays.
                }
            | tipo_especificador ID LSBRAC NUM RSBRAC SEMI
                {
                    $$ = newDeclNode(ArrayK);
                    $$->type = $1;
                    $$->attr.name = $2;
                    $$->child[0] = newExpNode(ConstK);
                    $$->child[0]->attr.val = $4;
                }
            ;
tipo_especificador : INT 
                {
                    $$ = Integer;
                }
            | VOID 
                {
                  $$ = Void;
                }
            ;

fun_declaracao : tipo_especificador ID LPAREN params RPAREN composto_decl 
                {
                  $$ = newDeclNode(FunK);
                  $$->type = $1;
                  $$->attr.name = $2;
                  $$->child[0] = $4; // Se for null é pq nao tem parametros (ver produção abaixo)
                  $$->child[1] = $6;
                }
            ;
params      : param_lista 
                {
                  $$ = $1;
                }
            | VOID
                {
                  $$ = NULL;
                }
            ;
param_lista : param_lista COMMA param
                {
                  TreeNode* t = $1;
                  if (t != NULL) {
                    while (t->sibling != NULL) t = t->sibling;
                    t->sibling = $3;
                    $$ = $1;
                  }
                  else $$ = $3;
                }
            | param
                {
                  $$ = $1;
                }
            ;
param       : tipo_especificador ID 
                {
                  $$ = newDeclNode(ParamK);
                  $$->type = $1;
                  $$->attr.name = $2;
                }
            | tipo_especificador ID LSBRAC RSBRAC
                {
                  $$ = newDeclNode(ParamK);
                  $$->type = $1;
                  $$->attr.name = $2;
                  // $$->attr.val = 1;
                  $$->arrayField = 1; 
                }
            ;
composto_decl : LCBRAC local_declaracoes statement_lista RCBRAC 
                {
                  // tentando fazer append lado a lado
                  TreeNode* t = $2;
                  if (t != NULL) {
                    while (t->sibling != NULL) t = t->sibling;
                    t->sibling = $3;
                    $$ = $2;
                  }
                  else {
                    $$ = $3;
                  }
                }
            ;
local_declaracoes : local_declaracoes var_declaracao 
                {
                  TreeNode* t = $1;
                  if (t != NULL) {
                    while (t->sibling != NULL) t = t->sibling;
                    t->sibling = $2;
                    $$ = $1;
                  }
                  else {
                    $$ = $2;
                  }
                }
            |
                {
                  $$ = NULL;
                }
            ;
statement_lista : statement_lista statement 
                {
                  TreeNode* t = $1;
                  if (t != NULL) {
                    while (t->sibling != NULL) t = t->sibling;
                    t->sibling = $2;
                    $$ = $1;
                  }
                  else {
                    $$ = $2;
                  }
                }
            | 
                {
                  $$ = NULL;
                } 
            ;
statement   : expressao_decl
                {
                  $$ = $1;
                }
            | composto_decl
                {
                  $$ = $1;
                }
            | selecao_decl
                {
                  $$ = $1;
                }
            | iteracao_decl
                {
                  $$ = $1;
                }
            | retorno_decl
                {
                  $$ = $1;
                }
            ;
expressao_decl : expressao SEMI 
                {
                  $$ = $1;
                }
            | SEMI
                {
                  $$ = NULL;
                }
            ;
selecao_decl : IF LPAREN expressao RPAREN statement %prec LOWER_THAN_ELSE
                {
                  $$ = newStmtNode(IfK);
                  $$->child[0] = $3;
                  $$->child[1] = $5;
                }
            | IF LPAREN expressao RPAREN statement ELSE statement
                {
                  $$ = newStmtNode(IfK);
                  $$->child[0] = $3;
                  $$->child[1] = $5;
                  $$->child[2] = $7;
                }
            ;
iteracao_decl : WHILE LPAREN expressao RPAREN statement
                {
                  $$ = newStmtNode(WhileK);
                  $$->child[0] = $3;
                  $$->child[1] = $5;
                }
            ;
retorno_decl : RETURN SEMI
                {
                  $$ = newStmtNode(ReturnK); 
                  // no child means empty return
                }
            | RETURN expressao SEMI
                {
                  $$ = newStmtNode(ReturnK);
                  $$->child[0] = $2;
                }
            ;
expressao   : var ASSIGN expressao
                {
                  $$ = newStmtNode(AssignK);
                  $$->child[0] = $1;
                  $$->child[0]->isFromAssign = 1;
                  $$->child[1] = $3;
                }
            | simples_expressao
                {
                  $$ = $1;
                }
            ;
var         : ID
                {
                  $$ = newExpNode(IdK);
                  $$->attr.name = $1;
                }
            | ID LSBRAC expressao RSBRAC
                {
                  $$ = newExpNode(ArrayIdK);
                  $$->attr.name = $1;
                  $$->child[0] = $3;
                }
            ;
simples_expressao : soma_expressao relacional soma_expressao
                {
                  $$ = newExpNode(OpK);
                  $$->attr.op = $2;
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                }
            | soma_expressao
                {
                  $$ = $1;
                }
            ;
relacional : LTE
                {
                  $$ = LTE;
                }
            | LT 
                {
                  $$ = LT;
                }
            | GT
                {
                  $$ = GT;
                }
            | GTE
                {
                  $$ = GTE;
                }
            | EQ
                {
                  $$ = EQ;
                }
            | DIFF
                {
                  $$ = DIFF;
                }
            ;
soma_expressao : soma_expressao soma termo
                {
                  $$ = newExpNode(OpK);
                  $$->attr.op = $2;
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                }
            | termo
                {
                  $$ = $1;
                }
            ;
soma        : PLUS 
                {
                  $$ = PLUS;
                }
            | MINUS
                {
                  $$ = MINUS;
                }
            ;
termo       : termo mult fator
                {
                  $$ = newExpNode(OpK);
                  $$->attr.op = $2;
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                }
            | fator
                {
                  $$ = $1;
                }
            ;
mult        : TIMES 
                {
                  $$ = TIMES;
                }
            | OVER 
                {
                  $$ = OVER;
                }
            ;
fator       : LPAREN expressao RPAREN 
                {
                  $$ = $2;
                }
            | var
                {
                  $$ = $1;
                } 
            | ativacao
                {
                  $$ = $1;
                }
            | NUM
                {
                  $$ = newExpNode(ConstK);
                  $$->attr.val  = $1;
                }
            ;
ativacao    : ID LPAREN args RPAREN
                {
                  $$ = newExpNode(ActvK);
                  $$->attr.name = $1;
                  $$->child[0] = $3;
                }
            ;
args        : arg_lista
                {
                  $$ = $1;
                }
            |  
                {
                  $$ = NULL;
                }
            ;
arg_lista   : arg_lista COMMA expressao
                {
                  TreeNode* t = $1;
                  if (t != NULL) {
                    while (t->sibling != NULL) t = t->sibling;
                    t->sibling = $3;
                    $$ = $1;
                  }
                  else {
                    $$ = $3;
                  }
                }
            | expressao
                {
                  $$ = $1;
                }
            ;
%%

int yyerror(char * message)
{ pce("Syntax error at line %d: %s\n",lineno,message);
  pce("Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}

