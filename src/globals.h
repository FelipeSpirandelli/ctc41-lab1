/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for TINY compiler          */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "log.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 8

typedef int TokenType;

extern FILE *source;  /* source code text file */
extern FILE *listing; /* listing output text file */
extern FILE *code;    /* code text file for TM simulator */
extern FILE *redundant_source;
extern int lineno; /* source line number for listing */

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum
{
  StmtK,
  ExpK,
  DeclK
} NodeKind;

// Statment => If | While | Assign | Read | Write | Return | Block
// Change of scope => If | While | Block
// Usually nodes with children
typedef enum
{
  IfK,
  WhileK,
  AssignK,
  ReadK,
  WriteK,
  ReturnK,
  BlockK
} StmtKind;

// Expression => Op | Const | Id | ArrayId | Actv
// Arithmetic expresions
typedef enum
{
  OpK,
  ConstK,
  IdK,
  ArrayIdK,
  ActvK
} ExpKind;

// Declaration => Var | Fun | Param | Array
// Change of scope => Fun
typedef enum
{
  VarK,
  FunK,
  ParamK,
  ArrayK
} DeclKind;

/* ExpType is used for type checking */
typedef enum
{
  Void,
  Integer,
  Boolean
} ExpType;

#define MAXCHILDREN 3
#define GLOBAL_SCOPE ""
#define PROTECTED_SCOPE "PROTECTED"

typedef struct treeNode
{
  struct treeNode *child[MAXCHILDREN];
  struct treeNode *sibling;
  // int isFromAssign;
  int lineno;
  int arrayField; // pra armazenar valores de arrays quando necessário
  NodeKind nodekind;
  union
  {
    StmtKind stmt;
    ExpKind exp;
    DeclKind decl;
  } kind; // uses new<type>Node on ./utils.c to create
  union
  {
    TokenType op;
    int val;
    char *name;
  } attr; // Usually used for regex
  ExpType type; /* for type checking of exps. Also for types of declarations */
} TreeNode;

#ifndef YYPARSER
#include "parser.h"
#define ENDFILE 0
#endif
/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error;
#endif
