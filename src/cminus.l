/****************************************************/
/* File: cminus.l                                   */
/* Lex specification for C-                         */
/* Project for CES41: Compiladores                  */
/****************************************************/

%option noyywrap 
/* opção noyywrap pode ser necessária para novas versões do flex
  limitação: não compila mais de um arquivo fonte de uma só vez (não precisamos disso)
  https://stackoverflow.com/questions/1480138/undefined-reference-to-yylex 
*/ 

%{
#include "globals.h"
#include "util.h"
#include "scan.h"
#include <stdio.h>
/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
%}
digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+
%%
"if"            {return IF;}
"else"          {return ELSE;}
"int"          {return INT;}
"return"          {return RETURN;}
"void"          {return VOID;}
"while"          {return WHILE;}

"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}

"<"             {return LT;}
"<="             {return LTE;}
">"             {return GT;}
">="             {return GTE;}
"=="             {return EQ;}
"!="             {return DIFF;}
"="             {return ASSIGN;}
";"             {return SEMI;}
","             {return COMMA;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"["             {return LSBRAC;}
"]"             {return RSBRAC;}
"{"             {return LCBRAC;}
"}"             {return RCBRAC;}

{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
"/*"            {
                    char c;
                    int foundAst = 0;
                    while (1) {
                        c = input();
                        if (c == EOF) break;
                        if (c == '*') {
                            foundAst = 1;
                            continue;
                        }
                        if (c == '/' && foundAst == 1) break;
                        if (c == '\n') lineno++;
                        foundAst = 0;
                    }
                }
.               {return ERROR;}
%%
TokenType getToken(void){
  static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    pc("\t%d: ",lineno);
    printToken(currentToken,tokenString);
  }
  
  return currentToken;
}
