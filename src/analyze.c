#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

/* counter for variable memory locations */
#define MAX_SCOPE_LEVEL 100

static int hasMainAppeared = 0;
static int memloc = 0;
static int sizeOfVariables = 0;
Context *contextStack[MAX_SCOPE_LEVEL];
int contextLevel;

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
    // pc("Traversing line %d with kind %d \n", t->lineno, t->nodekind);
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

void postProcScope(TreeNode *t)
{
  switch (t->nodekind)
  {
  case StmtK:
    switch (t->kind.stmt)
    {
    case WhileK:
    case IfK:
    case BlockK:
      free(contextStack[contextLevel]->scopeName);
      free(contextStack[contextLevel]);
      contextStack[contextLevel] = NULL;
      contextLevel--;
      break;
    default:
      break;
    }
    break;
  case DeclK:
    switch (t->kind.decl)
    {
    case FunK:
      st_set_scope_size(contextStack[contextLevel]->scopeName, sizeOfVariables);
      free(contextStack[contextLevel]->scopeName);
      free(contextStack[contextLevel]);
      contextStack[contextLevel] = NULL;
      contextLevel--;
      break;
    default:
      break;
    }
  default:
    break;
  }
}

void preProcScope(TreeNode *t, int doCreate)
{
  switch (t->nodekind)
  {
  case StmtK:
    switch (t->kind.stmt)
    {
    case WhileK:
      contextStack[contextLevel + 1] = newContext(getStackName(contextStack[contextLevel]->scopeName, "W", contextStack[contextLevel]->countOfWhile++));
      contextLevel++;
      if (doCreate)
        st_scope_insert(contextStack[contextLevel]->scopeName, getParentScope(), t->type);
      break;
    case IfK:
      contextStack[contextLevel + 1] = newContext(getStackName(contextStack[contextLevel]->scopeName, "I", contextStack[contextLevel]->countOfIf++));
      contextLevel++;
      if (doCreate)
        st_scope_insert(contextStack[contextLevel]->scopeName, getParentScope(), t->type);
      break;
    case BlockK:
      contextStack[contextLevel + 1] = newContext(getStackName(contextStack[contextLevel]->scopeName, "B", contextStack[contextLevel]->countOfBlock++));
      contextLevel++;
      if (doCreate)
        st_scope_insert(contextStack[contextLevel]->scopeName, getParentScope(), t->type);
      break;
    default:
      break;
    }
    break;
  case DeclK:
    switch (t->kind.decl)
    {
    case FunK:
      contextStack[contextLevel + 1] = newContext(concatStrings(contextStack[contextLevel]->scopeName, t->attr.name));
      contextLevel++;
      if (doCreate)
        st_scope_insert(contextStack[contextLevel]->scopeName, getParentScope(), t->type);
    default:
      break;
    }
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
    case AssignK:
      // case ReadK: Not used on the grammar
      // case WriteK: Not used in grammar
      if (st_lookup(contextStack[contextLevel]->scopeName, t->attr.name, 0, t->kind.stmt) != -1)
      {
        // pc("Inserting %s in %s line %d\n", t->attr.name, contextStack[contextLevel]->scopeName, t->lineno);
        st_insert(contextStack[contextLevel]->scopeName, getParentScope(), t->attr.name, t->lineno, 0, t->kind.stmt, t->type, 0,0);
      }
      break;
    case ReturnK:
      checkReturn(contextStack[contextLevel]->scopeName, t->child[0] == NULL);
      break;
    default:
      break;
    }
    break;
  case DeclK:
    switch (t->kind.decl)
    {
    case VarK:
    case ArrayK:
      if (st_lookup(contextStack[contextLevel]->scopeName, t->attr.name, 1, t->kind.decl) == -1)
      {
        // pc("Inserting var %s in %s\n", t->attr.name, contextStack[contextLevel]->scopeName);
        st_insert(contextStack[contextLevel]->scopeName, getParentScope(), t->attr.name, t->lineno, memloc, t->kind.stmt, t->type, 1,0);
      }
      if (t->kind.decl == VarK)
      {
        memloc++;
        sizeOfVariables++;
      }
      else
      {
        memloc += t->child[0]->attr.val;
        sizeOfVariables += t->child[0]->attr.val;
      }
      break;
    case ParamK:
      if (st_lookup(contextStack[contextLevel]->scopeName, t->attr.name, 1, t->kind.decl) == -1)
      {
        // pc("Inserting var %s in %s\n", t->attr.name, contextStack[contextLevel]->scopeName);
        st_insert(contextStack[contextLevel]->scopeName, getParentScope(), t->attr.name, t->lineno, memloc, t->arrayField ? ArrayK : VarK, t->type, 1,1);
      }
      memloc++;
      sizeOfVariables++;
      break;
    case FunK:
      if (st_lookup(contextStack[contextLevel]->scopeName, t->attr.name, 1, t->kind.decl) == -1)
      {
        // pc("Inserting fun %s in %s in line %d\n", t->attr.name, contextStack[contextLevel]->scopeName, t->lineno);
        st_insert(contextStack[contextLevel]->scopeName, getParentScope(), t->attr.name, t->lineno, 1, t->kind.stmt, t->type, 1,0);
      }
      memloc = 0;
      sizeOfVariables = 0;
    default:
      break;
    }
  case ExpK:

    switch (t->kind.exp)
    {
    // case OpK: Not relevant
    // case Const: Not relevant
    case ActvK:
    case IdK:
    case ArrayIdK:
      // pc("Lookup line %d for %s in %s\n", t->lineno, t->attr.name, contextStack[contextLevel]->scopeName);
      if (st_lookup(contextStack[contextLevel]->scopeName, t->attr.name, 0, t->kind.exp) != -1)
      {
        // pc("Inserting %s in %s line %d\n", t->attr.name, contextStack[contextLevel]->scopeName, t->lineno);
        st_insert(contextStack[contextLevel]->scopeName, getParentScope(), t->attr.name, t->lineno, 1, t->kind.stmt, t->type, 0,0);
      }
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
  preProcScope(t, 1);
}

char *getParentScope()
{
  return (contextLevel > 0 ? contextStack[contextLevel - 1]->scopeName : PROTECTED_SCOPE);
}

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode *syntaxTree)
{
  contextStack[contextLevel] = newContext(GLOBAL_SCOPE);
  insertInputOutput();
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

    default:
      break;
    }
    break;
  case StmtK:
    switch (t->kind.stmt)
    {
    case AssignK:
      if (t->child[1]->nodekind == ExpK && t->child[1]->kind.exp == ActvK)
      {
        ExpType type = getExpTypeOfSymbol(GLOBAL_SCOPE, t->child[1]->attr.name);
        if (type != -1 && type != Integer)
          pce("Semantic error at line %d: invalid use of void expression\n", t->lineno, getExpTypeOfSymbol(GLOBAL_SCOPE));
      }
      break;
    default:
      break;
    }
    break;
  case DeclK:
    switch (t->kind.decl)
    {
    case FunK:
      if (strcmp(t->attr.name, "main") == 0)
      {
        hasMainAppeared = 1;
      }
      break;
    default:
      break;
    }
  default:
    break;
  }
}

/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode *syntaxTree)
{
  traverse(syntaxTree, nullProc, checkNode);
  if (!hasMainAppeared)
  {
    pce("Semantic error: undefined reference to 'main'\n");
    Error = TRUE;
  }
}