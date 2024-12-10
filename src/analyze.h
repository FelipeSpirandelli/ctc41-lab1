#ifndef _ANALYZE_H_
#define _ANALYZE_H_

#define MAX_SCOPE_LEVEL 100

typedef struct Context
{
  char *scopeName;
  int countOfWhile, countOfIf, countOfBlock;
} Context;

extern Context *contextStack[MAX_SCOPE_LEVEL];
extern int contextLevel;
/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode *);

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode *);

char* getParentScope();

void postProcScope(TreeNode *);
void preProcScope(TreeNode *, int);
#endif
