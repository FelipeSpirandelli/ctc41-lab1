/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
// #include "globals.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

/* the hash function */
static int hash(char *key)
{
  int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  {
    temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

/* the list of line numbers of the source
 * code in which a variable is referenced
 */
typedef struct LineListRec
{
  int lineno;
  struct LineListRec *next;
} *LineList;

/* The record in the bucket lists for
 * each variable, including name,
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
typedef struct BucketListRec
{
  char *name;
  DeclKind idType;
  ExpType expType;
  LineList lines;
  int memloc; /* memory location for variable */
  struct BucketListRec *next;
} *BucketList;

/* The record for each scope, including
 * the name of the scope, the hash table
 * for the symbols in the scope, and a
 * pointer to the next scope
 */
typedef struct ScopeBucketListRec
{
  char *scopeName;
  BucketList hashTable[SIZE];
  struct ScopeBucketListRec *next;
  struct ScopeBucketListRec *parent;
} *ScopeBucketList;

/* the hash table */
static ScopeBucketList hashTable[SIZE];

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(char *scope, char *parentScope, char *name, int lineno, int loc, DeclKind idType, ExpType expType, int isSameScope)
{
  // pc("Inserting %s in %s\n", name, scope);
  ScopeBucketList s = st_scope_insert(scope, parentScope);
  st_symbol_insert(s, name, lineno, loc, idType, expType, isSameScope);
} /* st_insert */

ScopeBucketList st_scope_insert(char *scope, char *parentScope)
{
  // pc("Creating scope %s with parent %s\n", scope, parentScope);
  // find parent
  int parentHash = hash(parentScope);
  ScopeBucketList parent = hashTable[parentHash];
  while ((parent != NULL) && (strcmp(parentScope, parent->scopeName) != 0))
    parent = parent->next;

  int scopeHash = hash(scope);
  ScopeBucketList s = hashTable[scopeHash];
  while ((s != NULL) && (strcmp(scope, s->scopeName) != 0))
    s = s->next;
  if (s == NULL) /* scope not yet in table */
  {
    // pc("Inserting scope %s\n", scope);
    s = (ScopeBucketList)malloc(sizeof(struct ScopeBucketListRec));
    s->scopeName = strdup(scope);
    for (int i = 0; i < SIZE; i++)
    {
      s->hashTable[i] = NULL;
    }
    s->next = hashTable[scopeHash];
    s->parent = parent;
    hashTable[scopeHash] = s;
  }
  // else
  // {
  //   pc("Scope %s already in table\n", scope);
  // }
  return s;
}

BucketList st_symbol_insert(ScopeBucketList curScope, char *name, int lineno, int loc, DeclKind idType, ExpType expType, int isSameScope)
{
  // pc("Looking for %s in line %d\n", name, lineno);
  int nameHash = hash(name);
  BucketList l = NULL;
  ScopeBucketList s = curScope;
  while (s != NULL)
  {
    l = s->hashTable[nameHash];
    // pc("Looking for %s in scope %s\n", name, s->scopeName);
    while ((l != NULL) && (strcmp(name, l->name) != 0))
      l = l->next;
    if (l != NULL)
      break;
    if (isSameScope)
    {
      break;
    }
    s = s->parent;
  }
  if (l == NULL && isSameScope) /* variable not yet in table */
  {
    l = (BucketList)malloc(sizeof(struct BucketListRec));
    l->name = strdup(name);
    l->lines = (LineList)malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->idType = idType;
    l->expType = expType;
    l->lines->next = NULL;
    l->next = curScope->hashTable[nameHash];
    curScope->hashTable[nameHash] = l;
  }
  else if (l == NULL && !isSameScope)
  {
    pc("ERROR: Variable %s not declared on scope or parent scope with name %s\n", name, curScope->scopeName);
    return NULL;
  }
  else if (l != NULL && isSameScope)
  {
    pc("ERROR: Variable %s already declared\n", name);
    return NULL;
  }
  else /* found in table, so just add line number */
  {
    // pc("Found %s in %s line %d\n", name, s->scopeName, lineno);
    LineList t = l->lines;
    if( t == NULL) {
      l->lines = (LineList)malloc(sizeof(struct LineListRec));
      l->lines->lineno = lineno;
      l->lines->next = NULL;
    } else {
      while (t->next != NULL)
        t = t->next;
      if (t->lineno != lineno)
      {
        t->next = (LineList)malloc(sizeof(struct LineListRec));
        t->next->lineno = lineno;
        t->next->next = NULL;
      }
    }
  }
  return l;
}

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup(char *scope, char *name, int isSameScope)
{
  int nameHash = hash(name);
  int scopeHash = hash(scope);
  ScopeBucketList s = hashTable[scopeHash];

  while (s != NULL)
  {
    // pc("Looking for %s in scope %s\n", name, s->scopeName);
    BucketList l = s->hashTable[nameHash];
    while ((l != NULL) && (strcmp(name, l->name) != 0))
      l = l->next;
    if (l != NULL)
      return l->memloc;
    if (isSameScope)
    {
      break;
    }
    s = s->parent;
  }
  if(!isSameScope){
    pc("ERROR: Variable %s not declared on scope %s or parents\n", name, scope);
  }
  return -1;
}

void insertInputOutput() {
  ScopeBucketList s = st_scope_insert(GLOBAL_SCOPE, PROTECTED_SCOPE);
  int inputHash = hash("input");
  int outputHash = hash("output");
  BucketList input = s->hashTable[inputHash];
  BucketList output = s->hashTable[outputHash];
  if (input == NULL)
  {
    input = (BucketList)malloc(sizeof(struct BucketListRec));
    input->name = strdup("input");
    input->lines = NULL;
    input->memloc = 0;
    input->idType = FunK;
    input->expType = Integer;
    input->next = s->hashTable[inputHash];
    s->hashTable[inputHash] = input;
  }
  if (output == NULL)
  {
    output = (BucketList)malloc(sizeof(struct BucketListRec));
    output->name = strdup("output");
    output->lines = NULL;
    output->memloc = 0;
    output->idType = FunK;
    output->expType = Void;
    output->next = s->hashTable[outputHash];
    s->hashTable[outputHash] = output;
  }

}

/* Procedure printSymTab prints a formatted
 * list of the symbol table contents
 */
void printSymTab()
{
  int i;
  pc("Variable Name  Scope     ID Type  Data Type  Line Numbers\n");
  pc("-------------  --------  -------  ---------  -------------------------\n");
  for (i = 0; i < SIZE; ++i)
  {
    if (hashTable[i] != NULL)
    {
      ScopeBucketList s = hashTable[i];
      while (s != NULL)
      {
        for (int j = 0; j < SIZE; j++)
        {
          if (s->hashTable[j] != NULL)
          {
            BucketList l = s->hashTable[j];
            while (l != NULL)
            {
              LineList t = l->lines;
              pc("%-14s ", l->name);
              pc("%-8s  ", s->scopeName);
              pc("%-7s  ", getDeclKindString(l->idType));
              pc("%-9s  ", getExpTypeString(l->expType));
              while (t != NULL)
              {
                pc("%2d ", t->lineno);
                t = t->next;
              }
              pc("\n");
              l = l->next;
            }
          }
        }
        s = s->next;
      }
    }
  }
} /* printSymTab */
