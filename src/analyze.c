#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

/* counter for variable memory locations */
#define MAX_SCOPE_LEVEL 100
static int location = 0;

static int contextLevel = 0;
typedef struct Context
{
  char *scopeName;
  int countOfWhile, countOfIf, countOfBlock;
} Context;
Context *contextStack[MAX_SCOPE_LEVEL];

Context *newContext(char *scopeName)
{
  Context *context = (Context *)malloc(sizeof(Context));
  context->scopeName = strdup(scopeName);
  context->countOfWhile = 0;
  context->countOfIf = 0;
  context->countOfBlock = 0;
  return context;
}

/* Procedure traverse is a generic recursive
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc
 * in postorder to tree pointed to by t
 */
static void traverse(TreeNode *t,
                     void (*preProc)(TreeNode *),
                     void (*postProc)(TreeNode *))
{
  if (t != NULL)
  {
    preProc(t);
    {
      int i;
      for (i = 0; i < MAXCHILDREN; i++)
        traverse(t->child[i], preProc, postProc);
    }
    postProc(t);
    traverse(t->sibling, preProc, postProc);
  }
}

/* nullProc is a do-nothing procedure to
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode *t)
{
  if (t == NULL)
    return;
  else
    return;
}

static void postProcScope(TreeNode *t)
{
  switch (t->nodekind)
  {
  case StmtK:
    switch (t->kind.stmt)
    {
    case WhileK:
      free(contextStack[contextLevel]->scopeName);
      free(contextStack[contextLevel]);
      contextStack[contextLevel] = NULL;
      contextLevel--;
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

/* Procedure insertNode inserts
 * identifiers stored in t into
 * the symbol table
 */
static void insertNode(TreeNode *t)
{
  switch (t->nodekind)
  {
  case StmtK:
    switch (t->kind.stmt)
    {
    case WhileK:
      if(contextStack[contextLevel] == NULL) {
        pc("Context level %d is null\n", contextLevel);
        exit(EXIT_FAILURE);
      }
      pc("While %d\n", contextStack[contextLevel]->countOfWhile);
      contextStack[contextLevel + 1] = newContext(getStackName(contextStack[contextLevel]->scopeName, "WHILE", contextStack[contextLevel]->countOfWhile));
      contextLevel++;
      break;
    // case IfK:
    // case BlockK:
    // case ReturnK:
    //   break;
    case AssignK:
    // case ReadK: Not used on the grammar
    // case WriteK:
    pc("Context level %d and scope name %s with name %s\n", contextLevel, contextStack[contextLevel]->scopeName, t->attr.name);
      if (st_lookup(contextStack[contextLevel]->scopeName, t->attr.name, 0) != -1) {
        pc("Inserting %s in %s line %d\n", t->attr.name, contextStack[contextLevel]->scopeName, lineno);
        st_insert(contextStack[contextLevel]->scopeName, getParentScope(), t->attr.name, t->lineno, 0, t->kind.stmt, t->type, 0);
      }
    //   break;
    default:
      break;
    }
    break;
    case DeclK:
    switch (t->kind.decl) {
      case VarK:
      if (st_lookup(contextStack[contextLevel]->scopeName, t->attr.name, 1) == -1) {
        pc("Inserting %s in %s\n", t->attr.name, contextStack[contextLevel]->scopeName);
        st_insert(contextStack[contextLevel]->scopeName, getParentScope(), t->attr.name, t->lineno, location++, t->kind.stmt, t->type, 1);
      }
      break;
      default:
      break;
    }
  // case ExpK:
  //   switch (t->kind.exp)
  //   {
  //   case IdK:
  //     if (st_lookup(t->attr.name) == -1)
  //       /* not yet in table, so treat as new definition */
  //       st_insert(t->attr.name, t->lineno, location++);
  //     else
  //       /* already in table, so ignore location,
  //          add line number of use only */
  //       st_insert(t->attr.name, t->lineno, 0);
  //     break;
  //   default:
  //     break;
  //   }
  //   break;
  default:
    break;
  }
}

char* getParentScope() {
  return (contextLevel > 0 ? contextStack[contextLevel - 1]->scopeName : PROTECTED_SCOPE);
}

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode *syntaxTree)
{
  pc("Building Symbol Table...\n");
  contextStack[contextLevel] = newContext(GLOBAL_SCOPE);
  traverse(syntaxTree, insertNode, postProcScope);
  if (TraceAnalyze)
  {
    pc("\nSymbol table:\n\n");
    printSymTab();
  }
}

static void typeError(TreeNode *t, char *message)
{
  pce("Type error at line %d: %s\n", t->lineno, message);
  Error = TRUE;
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode *t)
{
  switch (t->nodekind)
  {
  case ExpK:
    switch (t->kind.exp)
    {
    case OpK:
      if ((t->child[0]->type != Integer) ||
          (t->child[1]->type != Integer))
        typeError(t, "Op applied to non-integer");
      // if ((t->attr.op == EQ) || (t->attr.op == LT))
      //   t->type = Boolean;
      // else
      //   t->type = Integer;
      break;
    case ConstK:
    case IdK:
      t->type = Integer;
      break;
    default:
      break;
    }
    break;
  case StmtK:
    switch (t->kind.stmt)
    {
    case IfK:
      if (t->child[0]->type == Integer)
        typeError(t->child[0], "if test is not Boolean");
      break;
    case AssignK:
      if (t->child[0]->type != Integer)
        typeError(t->child[0], "assignment of non-integer value");
      break;
    case WriteK:
      if (t->child[0]->type != Integer)
        typeError(t->child[0], "write of non-integer value");
      break;
    // case RepeatK:
    //   if (t->child[1]->type == Integer)
    //     typeError(t->child[1], "repeat test is not Boolean");
    //   break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode *syntaxTree)
{
  // traverse(syntaxTree, nullProc, checkNode);
}