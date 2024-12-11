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
#define PROLOGUE_CODE_LEN 5
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
static void genPrologue(TreeNode * tree, char* funcName) {
   int argCount = 0;
   int returnPC = 0;
   char* scopeName;
   ScopeMemLock loc;
   TreeNode * currentArg;
   if (TraceCode) emitComment("-> Function Prologue");
   emitRM("ST", fp, 0, sp, "Prologue: Storing FP on stack");
   // emitRM("LDA", fp, 0, sp, "Prologue: FP now points to current frame"); TODO: ADD THIS AFTER POPULATING THE ARGS
   emitRM("LDA", sp, -1, sp, "Prologue: Decrementing SP");
   // emitRM("ST", returnPC, 0, sp, "Prologue: Storing return address on stack"); TODO: ONLY RESTORE RETADDR AFTER KNOWING IT
   emitRM("LDA", sp, -1, sp, "Prologue: Decrementing SP");   
   int len = st_scope_lookup(funcName)->sizeOfVariables;
   // emitRM("LDA", sp, -len, sp, "Prologue: Allocating memory for variables and arguments"); TODO: only add this after populating args
   // Now we populate the args. they are siblings, so we dont call cGen
   currentArg = tree->child[0];

   // calculate them and store in the correct position on the new frame.
   while (currentArg != NULL) {
      // they should be expression nodes
      if (currentArg->nodekind != ExpK) {
         emitComment("ARG: ERROR. NOT AN EXPRESSION");
         return;
      }
      if (currentArg->kind.exp == IdK) {
         // check if is an array passed by reference
         scopeName = contextStack[contextLevel]->scopeName;
         loc = st_lookup_memloc(scopeName, tree->attr.name);
         if (loc.idType == ArrayK) {
            // array passed by reference
            genExp(currentArg, 1);
         } else {
            // just a normal int variable
            genExp(currentArg, 0);
         }
      } else {
         genExp(currentArg, 0);
      }
      emitRM("ST",ac, -(argCount++), sp, "Storing arg value after new stack pointer");
      currentArg = currentArg->sibling;
   }
   // Args stored. Now we can abandon the previous frame
   emitRM("LDA", fp, 2, sp, "Prologue: FP now points to current frame");
   // populate return address. we add 1 because of the 2 other instructions below
   returnPC = emitSkip(0) + 3;
   emitRM("LDC",ac, returnPC, ac, "Storing return address on ac");
   emitRM("ST", ac, -1, fp, "Store return address on stack" ); /* RM     mem(d+reg(s)) = reg(r) */
   emitRM("LDA", sp, -len, sp, "Prologue: Allocating memory for variables and arguments (arguments already populated)");
   // NOW JUMP TO THE FUNCTION THAT WAS JUST CALLED
   // REMEMBER TO INCREMENT RETURNPC BY 1

   if (TraceCode) emitComment("<- Function Prologue");  
}

/* Procedure genStmt generates code at a statement node */
static void genStmt(TreeNode *tree)
{
   TreeNode *p1, *p2, *p3;
   int savedLoc1, savedLoc2, currentLoc;
   ScopeMemLock loc;
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
      emitComment("if: jump to else belongs here"); // IF NO ELSE, JUMP TO END INSTEAD

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
         emitComment("-> while");
      p1 = tree->child[0];
      p2 = tree->child[1];
      savedLoc1 = emitSkip(0);
      emitComment("while: jump after body comes back here");
      /* generate code for test */
      cGen(p1);
      savedLoc2 = emitSkip(1); // Code to jump outside of loop goes here
      /* generate code for body */
      cGen(p2);
      emitRM_Abs("LDA", PC, savedLoc1, "Unconditional relative jmp to loop");
      // emitRM_Abs("JEQ", ac, savedLoc1, "while: jmp back to body");
      currentLoc = emitSkip(0);
      emitBackup(savedLoc2);
      emitRM_Abs("JEQ",ac,currentLoc,"while: jump to end of loop");
      emitRestore();
      if (TraceCode)
         emitComment("<- while");
      break; /* repeat */

   case AssignK:
      if (TraceCode)
         emitComment("-> assign");
      /* generate code for rhs */
      if (tree->child[0] == NULL) { // no array on left side
         cGen(tree->child[1]);
         /* now store value */
         char* scopeName = contextStack[contextLevel]->scopeName;
         loc = st_lookup_memloc(scopeName, tree->attr.name);
         pc("*current scope name: %s\n",scopeName);
         pc("*right variable scope: %s\n",loc.scopeName);

         if (!strcmp(loc.scopeName, GLOBAL_SCOPE)) {
            emitRM("ST", ac, loc.memloc, gp, "assign: store to global variable");
         } else {
            emitRM("ST", ac, -loc.memloc + FP_LOCALS_OFFSET, fp, "assign: store to local variable");
         }
      } else { // assign to array
         cGen(tree->child[1]);
         // store the result from right side temporarily on ac1
         emitRM("LDA",ac1,0,ac,"Saving temporary value on ac1");
         cGen(tree->child[0]); // ac now has the index of the left side array

         char* scopeName = contextStack[contextLevel]->scopeName;
         loc = st_lookup_memloc(scopeName, tree->attr.name); // returns beginning of array loc
         pc("*current scope name: %s\n",scopeName);
         pc("*right array variable scope: %s\n",loc.scopeName);

         if (!strcmp(loc.scopeName, GLOBAL_SCOPE)) {
            // right now, ac has the index, but we want it to be loc + idx, base gp
            emitRM("LDA", ac, loc.memloc, ac, "Loading relative global array index address into ac");
            emitRM("LDA", ac, 0, gp, "Loading absolute index address");
            emitRM("ST", ac1, 0, ac, "assign: store to global array"); /* RM     mem(d+reg(s)) = reg(r) */
         } else {
            emitRM("LDC", ac2, -loc.memloc, ac2, "loading array memloc on ac2");
            emitRO("SUB",ac,ac2,ac, "loading array index location on ac (relative to local_variables)");
            emitRO("ADD",ac,fp,ac, "adding fp to get index location on frame (except for FP_LOCALS_OFFSET)");
           //  emitRO("ADD", ac, ac, gp, "Loading absolute index address (except for FP_LOCALS_OFFSET)"); No need to add gp here
            emitRM("ST", ac1, FP_LOCALS_OFFSET, ac, "adding FP_LOCALS_OFFSET to get abslute index location");
         }
      }
      if (TraceCode)
         emitComment("<- assign");
      break; /* assign_k */

   // TODO: im not sure we are using ReadK nodes
   case ReadK:
      emitRO("IN", ac, 0, 0, "read integer value");
      // loc = st_lookup(tree->attr.name);
      emitRM("ST", ac, loc.memloc, gp, "read: store value");
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
void genExp(TreeNode *tree, int useAddress) // useAddress is used on activation calls when there is an array passed by reference
{
   char* scopeName;
   ScopeMemLock loc;
   TreeNode *p1, *p2;
   switch (tree->kind.exp)
   {

   case ConstK:
      if (TraceCode)
         //emitComment("-> Const");
      /* gen code to load integer constant using LDC */
      emitRM("LDC", ac, tree->attr.val, 0, "load const");
      if (TraceCode)
         //emitComment("<- Const");
      break; /* ConstK */

   case IdK:
      // TODO: IF CALLED WITH 1, RETURN THE ADDRESS ON ACINSTEAD OF VALUE
      if (TraceCode) emitComment("-> Id");
      scopeName = contextStack[contextLevel]->scopeName;
      loc = st_lookup_memloc(scopeName, tree->attr.name);

      pc("*current scope name: %s\n",scopeName);
      pc("*right variable scope: %s\n",loc.scopeName);
      if (!useAddress) {
         if (!strcmp(loc.scopeName, GLOBAL_SCOPE)) {
            // escopo global, offset de gp
            emitRM("LD", ac, loc.memloc, gp, "load id value");
         } else {
            // escopo local, offset de fp
            emitRM("LD", ac, -loc.memloc + FP_LOCALS_OFFSET, fp, "load local id value");
         }
      } else {
         if (!strcmp(loc.scopeName, GLOBAL_SCOPE)) {
            // i want to return gp + memloc
            emitRM("LDA", ac, loc.memloc, gp, "load global id address");
         } else {
            emitRM("LDA", ac, FP_LOCALS_OFFSET - loc.memloc, fp, "load local id address");
         }
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
      case LTE:
         emitRO("SUB", ac, ac, ac1, "op <");
         emitRM("JLE", ac, 2, PC, "br if true");
         emitRM("LDC", ac, 0, ac1, "false case");
         emitRM("LDA", PC, 1, PC, "unconditional jmp");
         emitRM("LDC", ac, 1, ac1, "true case");
         break;
      case GT:
         emitRO("SUB", ac, ac, ac1, "op <");
         emitRM("JGT", ac, 2, PC, "br if true");
         emitRM("LDC", ac, 0, ac1, "false case");
         emitRM("LDA", PC, 1, PC, "unconditional jmp");
         emitRM("LDC", ac, 1, ac1, "true case");
         break;
      case GTE:
         emitRO("SUB", ac, ac, ac1, "op <");
         emitRM("JGE", ac, 2, PC, "br if true");
         emitRM("LDC", ac, 0, ac1, "false case");
         emitRM("LDA", PC, 1, PC, "unconditional jmp");
         emitRM("LDC", ac, 1, ac1, "true case");
      case EQ:
         emitRO("SUB", ac, ac, ac1, "op ==");
         emitRM("JEQ", ac, 2, PC, "br if true");
         emitRM("LDC", ac, 0, ac1, "false case");
         emitRM("LDA", PC, 1, PC, "unconditional jmp");
         emitRM("LDC", ac, 1, ac1, "true case");
         break;
      case DIFF:
         emitRO("SUB", ac, ac, ac1, "op ==");
         emitRM("JNE", ac, 2, PC, "br if true");
         emitRM("LDC", ac, 0, ac1, "false case");
         emitRM("LDA", PC, 1, PC, "unconditional jmp");
         emitRM("LDC", ac, 1, ac1, "true case");
      default:
         emitComment("BUG: Unknown operator");
         break;
      } /* case op */
      if (TraceCode)
         emitComment("<- Op");
      break; /* OpK */

   case ArrayIdK:
      if (TraceCode)
         emitComment("-> Array Id");
      // ArrayIdK always return value. Check IdK on activations for arrays passed as references
      cGen(tree->child[0]);

      scopeName = contextStack[contextLevel]->scopeName;
      loc = st_lookup_memloc(scopeName, tree->attr.name);
      pc("*current scope name: %s\n",scopeName);
      pc("*right variable scope: %s\n",loc.scopeName);
      if (!strcmp(loc.scopeName, GLOBAL_SCOPE)) {
         // global array. we want mem[gp + loc + index]. at this point ac has index
         emitRO("ADD", ac, ac, gp, "ac = index + gp"); // ac = index + gp 
         /* RM     reg(r) = mem(d+reg(s)) */
         emitRM("LD", ac, loc.memloc, ac, "ac has the value");
      } else {
         // local array. we want reg(ac) = mem[fp + LOCALS_FP_OFFSET - loc - index]. index is on ac
         emitRO("SUB", ac, fp, ac, "ac = fp - index"); // ac = fp - index
         emitRM("LD", ac, -loc.memloc + FP_LOCALS_OFFSET, ac, "ac = mem[fp + FP_LOCALS_OFFSET - loc - index]");
      }

      if (TraceCode) emitComment("<- Array Id");
      break;
   case ActvK:
      // TODO: function prologue
      if (TraceCode)
         emitComment("-> Function Call");
         char* funcName = tree->attr.name;
         genPrologue(tree, funcName);
      if (TraceCode)
         emitComment("<- Function Call");
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
       if (TraceCode)
         emitComment("<- FunK");
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
         genExp(tree, 0);
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
