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
} *ScopeBucketList;

/* the hash table */
static ScopeBucketList hashTable[SIZE];

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(char* scope, char *name, int lineno, int loc)
{
  int scopeHash = hash(scope);
  int nameHash = hash(name);
  ScopeBucketList s = hashTable[scopeHash];
  BucketList l = s->hashTable[nameHash];
  while ((l != NULL) && (strcmp(name, l->name) != 0))
    l = l->next;
  if (l == NULL) /* variable not yet in table */
  {
    l = (BucketList)malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList)malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->lines->next = NULL;
    l->next = s->hashTable[nameHash];
    s->hashTable[nameHash] = l;
  }
  else /* found in table, so just add line number */
  {
    LineList t = l->lines;
    while (t->next != NULL)
      t = t->next;
    t->next = (LineList)malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
} /* st_insert */

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup(char *name)
{
  int h = hash(name);
  BucketList l = hashTable[h];
  while ((l != NULL) && (strcmp(name, l->name) != 0))
    l = l->next;
  if (l == NULL)
    return -1;
  else
    return l->memloc;
}

/* Procedure printSymTab prints a formatted
 * list of the symbol table contents
 */
void printSymTab()
{
  // int i;
  // pc("Variable Name  Location   Line Numbers\n");
  // pc("-------------  --------   ------------\n");
  // for (i = 0; i < SIZE; ++i)
  // {
  //   if (hashTable[i] != NULL)
  //   {
  //     BucketList l = hashTable[i];
  //     while (l != NULL)
  //     {
  //       LineList t = l->lines;
  //       pc("%-14s ", l->name);
  //       pc("%-8d  ", l->memloc);
  //       while (t != NULL)
  //       {
  //         pc("%4d ", t->lineno);
  //         t = t->next;
  //       }
  //       pc("\n");
  //       l = l->next;
  //     }
  //   }
  // }
} /* printSymTab */
