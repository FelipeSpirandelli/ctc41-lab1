/****************************************************/
/* File: cgen.c                                     */
/* The code generator implementation                */
/* for the TINY compiler                            */
/* (generates code for the TM machine)              */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"
#include "analyze.h"

/* tmpOffset is the memory offset for temps
   It is decremented each time a temp is
   stored, and incremeted when loaded again
*/
static int tmpOffset = 0;
static int savedMainJumpLoc = 0; // because main should run after global declaratins/assignments, but before the first function
static int isFirstFunction = 1;  // flag to help place the jump to main in the correct place


// FOR TESTING
#define INITIAL_SP 100
#define FP_LOCALS_OFFSET -2
/* prototype for internal recursive code generator */
static void cGen(TreeNode *tree);

static void genMainPrologue(TreeNode *tree) {
   // store current fp value in address pointed by sp. doesn't matter since is main
   //emitRM("ST", fp, 0, sp, "Storing FP on stack");
   // decrement sp
   emitRM("LDA", sp, -1, sp, "Decrementing SP");
   // store return address (pc+1). does not matter for main
   //emitRM("ST", PC+1, 0, sp, "Storing return address on stack");
   // decrement sp
   emitRM("LDA", sp, -1, sp, "Decrementing SP");
   //A main não tem argumentos. Pular para variáveis locais
   int len = st_scope_lookup("main")->sizeOfVariables;
   emitRM("LDA", sp, -len, sp, "Decrementing SP");
}

/* Procedure genStmt generates code at a statement node */
static void genStmt(TreeNode *tree)
{
   TreeNode *p1, *p2, *p3;
   int savedLoc1, savedLoc2, currentLoc;
   int loc;
   switch (tree->kind.stmt)
   {

   case IfK:
      if (TraceCode)
         emitComment("-> if");
      p1 = tree->child[0];
      p2 = tree->child[1];
      p3 = tree->child[2];
      /* generate code for test expression */
      cGen(p1);
      savedLoc1 = emitSkip(1);
      emitComment("if: jump to else belongs here");
      /* recurse on then part */
      cGen(p2);
      savedLoc2 = emitSkip(1);
      emitComment("if: jump to end belongs here");
      currentLoc = emitSkip(0);
      emitBackup(savedLoc1);
      emitRM_Abs("JEQ", ac, currentLoc, "if: jmp to else");
      emitRestore();
      /* recurse on else part */
      cGen(p3);
      currentLoc = emitSkip(0);
      emitBackup(savedLoc2);
      emitRM_Abs("LDA", PC, currentLoc, "jmp to end");
      emitRestore();
      if (TraceCode)
         emitComment("<- if");
      break; /* if_k */

   case WhileK:
      if (TraceCode)
         emitComment("-> repeat");
      p1 = tree->child[0];
      p2 = tree->child[1];
      savedLoc1 = emitSkip(0);
      emitComment("repeat: jump after body comes back here");
      /* generate code for body */
      cGen(p1);
      /* generate code for test */
      cGen(p2);
      emitRM_Abs("JEQ", ac, savedLoc1, "repeat: jmp back to body");
      if (TraceCode)
         emitComment("<- repeat");
      break; /* repeat */

   case AssignK:
      if (TraceCode)
         emitComment("-> assign");
      /* generate code for rhs */
      cGen(tree->child[1]);
      /* now store value */
      char* scopeName = contextStack[contextLevel]->scopeName;
      loc = st_lookup_memloc(scopeName, tree->attr.name);
      if (!strcmp(scopeName, GLOBAL_SCOPE)) {
         emitRM("ST", ac, loc, gp, "assign: store to global variable");
      } else {
         emitRM("ST", ac, -loc + FP_LOCALS_OFFSET, fp, "assign: store to local variable");

      }
      if (TraceCode)
         emitComment("<- assign");
      break; /* assign_k */

   // TODO: im not sure we are using ReadK nodes
   case ReadK:
      emitRO("IN", ac, 0, 0, "read integer value");
      // loc = st_lookup(tree->attr.name);
      emitRM("ST", ac, loc, gp, "read: store value");
      break;

   // TODO: im not sure we are using WriteK nodes
   case WriteK:
      /* generate code for expression to write */
      cGen(tree->child[0]);
      /* now output it */
      emitRO("OUT", ac, 0, 0, "write ac");
      break;

   // TODO: Execute the function epilogue
   case ReturnK:
      if (tree->child[0] != NULL)
      {
         // If has an expression, hopefully the result will be in ac
         cGen(tree->child[0]);
      }
      // Execute epilogue
      break;

   case BlockK:
      // TODO: do something with scope
   default:
      break;
   }
} /* genStmt */

/* Procedure genExp generates code at an expression node */
static void genExp(TreeNode *tree)
{
   int loc;
   TreeNode *p1, *p2;
   switch (tree->kind.exp)
   {

   case ConstK:
      if (TraceCode)
         emitComment("-> Const");
      /* gen code to load integer constant using LDC */
      emitRM("LDC", ac, tree->attr.val, 0, "load const");
      if (TraceCode)
         emitComment("<- Const");
      break; /* ConstK */

   case IdK:
      if (TraceCode) emitComment("-> Id");
      char* scopeName = contextStack[contextLevel]->scopeName;
      loc = st_lookup_memloc(scopeName, tree->attr.name);
      if (!strcmp(scopeName, GLOBAL_SCOPE)) {
         // escopo global, offset de gp
         emitRM("LD", ac, loc, gp, "load id value");
      } else {
         // escopo local, offset de fp
         emitRM("LD", ac, -loc + FP_LOCALS_OFFSET, fp, "load local id value");
      }
      if (TraceCode)
         emitComment("<- Id");
      break; /* IdK */

   case OpK:
      if (TraceCode)
         emitComment("-> Op");
      p1 = tree->child[0];
      p2 = tree->child[1];
      /* gen code for ac = left arg */
      cGen(p1);
      /* gen code to push left operand */
      //emitRM("ST", ac, tmpOffset--, mp, "op: push left");
      emitRM("LDA",ac1,0,ac,"Saving temporary value on ac1");
      /* gen code for ac = right operand */
      cGen(p2);
      /* now load left operand */
      //emitRM("LD", ac1, ++tmpOffset, mp, "op: load left");
      switch (tree->attr.op)
      {
      case PLUS:
         emitRO("ADD", ac, ac, ac1, "op +");
         break;
      case MINUS:
         emitRO("SUB", ac, ac, ac1, "op -");
         break;
      case TIMES:
         emitRO("MUL", ac, ac, ac1, "op *");
         break;
      case OVER:
         emitRO("DIV", ac, ac, ac1, "op /");
         break;
      case LT:
         emitRO("SUB", ac, ac, ac1, "op <");
         emitRM("JLT", ac, 2, PC, "br if true");
         emitRM("LDC", ac, 0, ac1, "false case");
         emitRM("LDA", PC, 1, PC, "unconditional jmp");
         emitRM("LDC", ac, 1, ac1, "true case");
         break;
      case EQ:
         emitRO("SUB", ac, ac, ac1, "op ==");
         emitRM("JEQ", ac, 2, PC, "br if true");
         emitRM("LDC", ac, 0, ac1, "false case");
         emitRM("LDA", PC, 1, PC, "unconditional jmp");
         emitRM("LDC", ac, 1, ac1, "true case");
         break;
      default:
         emitComment("BUG: Unknown operator");
         break;
      } /* case op */
      if (TraceCode)
         emitComment("<- Op");
      break; /* OpK */

   case ArrayIdK:
      if (TraceCode)
         emitComment("-> ArrayIdK");
      // get the result of what is inside
      cGen(tree->child[0]);
      // TODO
      // return either the address or the value. Depends on its parents.
      // Example: if its parent is a Activation node, return the address. If it is in the right side of an assign expression, return the value in that address.
      // Maybe add another parameter in the recursive function
      break;
   case ActvK:
      // TODO: function prologue
      break;
   default:
      break;
   }
} /* genExp */
static void genDecl(TreeNode *tree)
{
   int savedLoc;
   switch (tree->kind.decl)
   {
   case FunK:
      // TODO: use child[0] to build the call frame
      if (TraceCode)
         emitComment("-> FunK");

      if (isFirstFunction)
      {
         if (!strcmp(tree->attr.name, "main"))
         { // main is first function
            // gen main prologue
            genMainPrologue(tree);
         }
         else
         { // regular function is first
            savedMainJumpLoc = emitSkip(1);
            isFirstFunction = 0;
         }
      }
      else
      { // not first function

         if (!strcmp(tree->attr.name, "main"))
         { // main acheived, but is not first function
            savedLoc = emitSkip(0);
            emitBackup(savedMainJumpLoc);
            emitRM_Abs("LDA", PC, savedLoc, "Unconditional relative jmp to main");
            emitRestore();
            // gen main prologue
            genMainPrologue(tree);

         }
         else
         { // regular function
            // do something if needed
         }
      }
      // gen code
      cGen(tree->child[1]);
      break;
   default:
      break;
   }
}

/* Procedure cGen recursively generates code by
 * tree traversal
 */
static void cGen(TreeNode *tree)
{
   if (tree != NULL)
   {
      preProcScope(tree, 0);
      switch (tree->nodekind)
      {
      case DeclK:
         genDecl(tree);
         break;
      case StmtK:
         genStmt(tree);
         break;
      case ExpK:
         genExp(tree);
         break;
      default:
         break;
      }
      postProcScope(tree);
      cGen(tree->sibling);
   }
}

/**********************************************/
/* the primary function of the code generator */
/**********************************************/
/* Procedure codeGen generates code to a code
 * file by traversal of the syntax tree. The
 * second parameter (codefile) is the file name
 * of the code file, and is used to print the
 * file name as a comment in the code file
 */
void codeGen(TreeNode *syntaxTree)
{
   emitComment("TINY Compilation to TM Code");
   /* generate standard prelude */
   emitComment("Standard prelude:");
   emitRM("LD", mp, 0, ac, "load maxaddress from location 0");
   emitRM("ST", ac, 0, ac, "clear location 0");
   emitRM("LDC", sp, INITIAL_SP, sp, "Initial SP value"); // TODO: Remove this later because it will be in MP
   emitComment("End of standard prelude.");
   /* generate code for TINY program */
   cGen(syntaxTree);
   /* finish */
   emitComment("End of execution.");
   emitRO("HALT", 0, 0, 0, "");
}
