/****************************************************/
/* File: symtab.h                                   */
/* Symbol table interface for the TINY compiler     */
/* (allows only one symbol table)                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "log.h"
#include "globals.h"
#include "util.h"

typedef struct BucketListRec *BucketList;
typedef struct ScopeBucketListRec *ScopeBucketList;

// I need to insert: lookup on scope and insert on scope
// I need to use: lookup on all parent scopes and insert line on first found

/* Procedure st_insert inserts scope
 * into the scope table and symbol
 * on the symbol table of the scope
 */
void st_insert(char *scope, char *parentScope, char *name, int lineno, int loc, DeclKind idType, ExpType expType, int isSameScope);

/*
 * Procedure st_scope_insert inserts scope
 * into the scope table
 */
ScopeBucketList st_scope_insert(char *name, char *parentScope, ExpType returnType);

/*
 * Procedure st_symbol_insert inserts symbol
 * into the symbol table of the scope
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
BucketList st_symbol_insert(ScopeBucketList curScope, char *name, int lineno, int loc, DeclKind idType, ExpType expType, int isSameScope);

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup(char *scope, char *name, int isSameScope);

void checkReturn(char *scope, int isNull);

/* Procedure printSymTab prints a formatted
 * list of the symbol table contents
 */
void printSymTab();

#endif
