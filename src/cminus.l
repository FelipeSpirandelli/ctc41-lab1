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
char line[4096];
%}
digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     [\n]
whitespace  [ \t\r]+
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

{number}        {/*yylval.val = atoi(yytext)*/; return NUM;}
{identifier}    {/*yylval.name = copyString(yytext)*/; return ID;}
{newline}       {lineno++;printLine();}
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
                        if (c == '\n'){ lineno++;printLine();}
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
    printLine();
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    pc("\t%d: ",lineno);
    printToken(currentToken,tokenString);
  }
  
  return currentToken;
}

void printLine(void){
  char line[1024];
  char *ret = fgets(line, 1024, redundant_source);
  // If an error occurs, or if end-of-file is reached and no characters were read, fgets returns NULL.
  if(ret){ 
    pc( "%d: %-1s",lineno, line); 
    // if EOF, the string does not contain \n. add it to separate from EOF token
    if (feof(redundant_source)) pc("\n");
  } 
}
