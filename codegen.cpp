#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treeNodes.h"
#include "treeUtils.h"
#include "symbolTable.h"
#include "emitcode.h"
#include "parser.tab.h"
#include "codegen.h"
using namespace std;

// These offsets that never change
#define OFPOFF 0
#define RETURNOFFSET -1

extern int numErrors;
extern int numWarnings;
extern int yydebug;
extern char **largerTokens;
//extern void yyparse();
extern void initTokenStrings();
extern TreeNode *syntaxTree;

int toffset; // next available temporary space


FILE *code;
static bool linenumFlag;
static int breakloc = 0;
static SymbolTable *globals;

//void codegenHeader(char *srcFile);
//int offsetRegister(VarKind v);
//void codegenStmt(TreeNode *current);
//void codegenDecl(TreeNode *current);
//void codegenExp(TreeNode *current);
//void codegenGeneral(TreeNode *current);
//void commentLineNum(TreeNode *current);
//void codegenLibraryFun(TreeNode *current);
//void codegenFun(TreeNode *current);

// This is the top level generator call
void codegen(FILE *codeIn,             // where the code should be written
             char *srcFile,            // name of file compiled
             TreeNode *syntaxTree,     // tree to process
             SymbolTable *globalsIn,   // globals so function info can be found
             int globalOffset,         // size of the global frame
             bool linenumFlagIn) {     // comment with line numbers
   int initJump;

   code = codeIn;
   globals = globalsIn;
   linenumFlag = linenumFlagIn;
   breakloc = 0;
   
   initJump = emitSkip(1);                // save a place for the jump to init
   codegenHeader(srcFile);                // nice comments describing what is compiled
   codegenGeneral(syntaxTree);            // general code generation including I/O library
   codegenInit(initJump, globalOffset);   // generation of initialization for run

   return;
}

void codegenHeader(char *srcFile) {
   emitComment((char *)"bC compiler version bC-Su23");
   emitComment((char *)"File compiled: ", srcFile);
   return;
}

void codegenGeneral(TreeNode *current) {
   //printf("Made it into codegenGeneral ===================\n");
   while (current) {
      //printf("In while ===================\n");
      switch (current->nodekind) {
         case StmtK:
            //emitComment((char *)"StmtK");
            codegenStmt(current);
            break;
         case ExpK:
            emitComment((char *)"EXPRESSION");
            codegenExp(current); // Error coming from here
            break;
         case DeclK:
            //emitComment((char *)"DeclK");
            codegenDecl(current);
            break;
      }
      current = current->sibling;
   }
}

// Helper function for IdK, AssignK, and VarK
int offsetRegister(VarKind v) {
   switch (v) {
      case Local:
         return FP;
      case Parameter:
         return FP;
      case Global:
         return GP;
      case LocalStatic:
         return GP;
      default:
         printf((char *)"ERROR(codegen): Looking up offset register for a variable of type %d\n", v);
         return 666;
   }
}

void commentLineNum(TreeNode *current) {
   char buffer[16];

   if (linenumFlag) {
      sprintf(buffer, "%d", current->lineno);
      emitComment((char*)"Line: ", buffer);
   }
   return;
}

void codegenLibraryFun(TreeNode *current) {
   emitComment((char *)"");
   emitComment((char *)"** ** ** ** ** ** ** ** ** ** ** **");
   emitComment((char *)"FUNCTION", current->attr.name);

   // Remember where this function is
   current->offset = emitSkip(0);
   
   // Store return address
   emitRM((char *)"ST", AC, RETURNOFFSET, FP, (char *)"Store return address");
   
   if (strcmp(current->attr.name, (char *)"input") == 0) {
      emitRO((char *)"IN", RT, RT, RT, (char *)"Grab int input");
   }
   else if (strcmp(current->attr.name, (char *)"inputb") == 0) {
      emitRO((char *)"INB", RT, RT, RT, (char *)"Grab bool input");
   }
   else if (strcmp(current->attr.name, (char *)"inputc") == 0) {
      emitRO((char *)"INC", RT, RT, RT, (char *)"Grab char input");
   }
   else if (strcmp(current->attr.name, (char *)"output") == 0) {
      emitRM((char *)"LD", AC, -2, FP, (char *)"Load parameter");
      emitRO((char *)"OUT", AC, AC, AC, (char *)"Output integer");
   }
   else if (strcmp(current->attr.name, (char *)"outputb") == 0) {
      emitRM((char *)"LD", AC, -2, FP, (char *)"Load parameter");
      emitRO((char *)"OUTB", AC, AC, AC, (char *)"Output bool");
   }
   else if (strcmp(current->attr.name, (char *)"outputc") == 0) {
      emitRM((char *)"LD", AC, -2, FP, (char *)"Load parameter");
      emitRO((char *)"OUTC", AC, AC, AC, (char *)"Output char");
   }
   else if (strcmp(current->attr.name, (char *)"outnl") == 0) {
      emitRO((char *)"OUTNL", AC, AC, AC, (char *)"Output a newline");
   }
   else {
      emitComment((char *)"ERROR(LINKER): No support for special function");
      emitComment(current->attr.name);      
   }

   emitRM((char *)"LD", AC, RETURNOFFSET, FP, (char *)"Load return address"); 
   emitRM((char *)"LD", FP, OFPOFF, FP, (char *)"Adjust fp");
   emitGoto(0, AC, (char *)"Return");

   emitComment((char *)"END FUNCTION", current->attr.name);
   return;
}

void codegenFun(TreeNode *current) {
   emitComment((char *)"");
   emitComment((char *)"** ** ** ** ** ** ** ** ** ** ** **");
   emitComment((char *)"FUNCTION", current->attr.name);
   toffset = current->size;
   emitComment((char *)"TOFF set:", toffset);

   // IMPORTANT: For function nodes, the offset is defined to be the position
   // of the function in the code space! This is accessible via the symbol table.
   // Remember where this function is:
   current->offset = emitSkip(0);

   // Store return address
   emitRM((char *)"ST", AC, RETURNOFFSET, FP, (char *)"Store return address");

   // Generate code for the statements...
   codegenGeneral(current->child[1]);

   // In coase there was no return statement
   // Set return register to 0 and return
   emitComment((char *)"Add standard closing in case there is no return statement");
   emitRM((char *)"LDC", RT, 0, 6, (char *)"Set return value to 0");
   emitRM((char *)"LD", AC, RETURNOFFSET, FP, (char *)"Load return address");
   emitRM((char *)"LD", FP, OFPOFF, FP, (char *)"Adjust fp");
   emitGoto(0, AC, (char *)"Return");

   emitComment((char *)"END FUNCTION", current->attr.name);

   return;
}

void codegenInit(int initJump, int globalOffset) {
   backPatchAJumpToHere(initJump, (char *)"Jump to init [backpatch]");

   emitComment((char *)"INIT");
   emitRM((char *)"LDA", FP, globalOffset, GP, (char *)"set first frame at end of globals");
   emitRM((char *)"ST", FP, 0, FP, (char *)"store old fp (point to self)");

   initGlobalArraySizes();

   emitRM((char *)"LDA", AC, 1, PC, (char *)"Return address in ac");

   {  // Jump to main
      TreeNode *funcNode;

      funcNode = (TreeNode *)(globals->lookup((char *)"main"));
      if (funcNode) {
         emitGotoAbs(funcNode->offset, (char *)"Jump to main");
      }
      else {
         printf((char *)"ERROR(LINKER): Procedure main is not defined.\n");
         numErrors++;
      }
   }

   emitRO((char *)"HALT", 0, 0, 0, (char *)"DONE!");
   emitComment((char *)"END INIT");
   return;
}

void initGlobalArraySizes() {
   emitComment((char *)"INIT GLOBALS AND STATICS");
   globals->applyToAllGlobal(initAGlobalSymbol);
   emitComment((char *)"END INIT GLOBALS AND STATICS");
   return;
}

void initAGlobalSymbol(std::string sym, void *ptr) {
   TreeNode *current;
   
   // printf("Symbol: %s\n", sym.c_str()); // dump the symbol table
   current = (TreeNode *)ptr;

   // printf("lineno: %d\n", current->lineno); // dump the symbol table
   if (current->lineno != -1) {
      if (current->isArray) {
         emitRM((char *)"LDC", AC, current->size-1, 6, (char *)"load size of array", current->attr.name);
         emitRM((char *)"ST", AC, current->offset+1, GP, (char *)"save size of array", current->attr.name);
      }

      if (current->kind.decl==VarK && (current->varKind == Global || current->varKind == LocalStatic)) {
         if (current->child[0]) {
            // compute rhs -> AC;
            codegenExp(current->child[0]);

            // save it
            emitRM((char *)"ST", AC, current->offset, GP, (char *)"Store variable", current->attr.name);
         }
      }
   }

   return;
}

void codegenStmt(TreeNode *current) {
   if (current == NULL) {
      return;
   }

   int skiploc = 0, skiploc2 = 0, currloc = 0, offReg;
   TreeNode *loopindex=NULL;
   TreeNode *lhs, *rhs;
   lhs = current->child[0];
   rhs = current->child[1];

   commentLineNum(current);

   switch(current->kind.stmt) {
      case IfK:
         emitComment((char *)"IF");
         codegenExp(lhs);  // process inits

         skiploc = emitSkip(1);
         emitComment((char *)"THEN");
         codegenGeneral(rhs);  // process body

         if (current->child[2] != NULL) {
            skiploc2 = emitSkip(1);
         }

         backPatchAJumpToHere((char *)"JZR", AC, skiploc, (char *)"Jump around the THEN if false [backpatch]");

         if (current->child[2] != NULL) {
            emitComment((char *)"ELSE");
            codegenGeneral(current->child[2]);
            backPatchAJumpToHere(skiploc2, (char *)"Jump around the ELSE [backpatch]");
         }

         emitComment((char *)"END IF");
         break;
      case WhileK:
         emitComment((char *)"WHILE");
         currloc = emitSkip(0);  // Return to here to do the test
         codegenExp(lhs);  // Test expression

         emitRM((char *)"JNZ", AC, 1, PC, (char *)"Jump to while part");
         emitComment((char *)"DO");

         skiploc = breakloc;     // Save the old break statement return point
         breakloc = emitSkip(1); // Addr of instr that jumps to end of loop. This is also the backpatch point

         codegenGeneral(rhs);    // Do body of loop
         emitGotoAbs(currloc, (char *)"go to beginning of loop");
         backPatchAJumpToHere(breakloc, (char *)"Jump past loop [backpatch]");   // backpatch jump to the end of the loop

         breakloc = skiploc;
         emitComment((char *)"END WHILE");

         break;
      case ForK:
         int savedToffset;
         int startoff, stopoff, stepoff;
         savedToffset = toffset;
         toffset = current->size;

         emitComment((char *)"TOFF set:", toffset);
         emitComment((char *)"FOR");

         // ==============================================
         loopindex = lhs;
         startoff = loopindex->offset;
         stopoff = startoff-1;
         stepoff = startoff-2;

         TreeNode *rangenode;
         rangenode = rhs;

         codegenExp(rangenode->child[0]);            
         emitRM((char *)"ST", AC, startoff, FP, (char *)"save starting value in index variable");
         codegenExp(rangenode->child[1]);
         emitRM((char *)"ST", AC, stopoff, FP, (char *)"save stop value");

         if (rangenode->child[2] != NULL) {
            codegenExp(rangenode->child[2]);
         } else {
         // ==============================================
            emitRM((char *)"LDC", AC, 1, 6, (char *)"default increment by 1");
         }
         emitRM((char *)"ST", AC, stepoff, FP, (char *)"save step value");

         currloc = emitSkip(0);
         
         //codegenGeneral(lhs);
         //emitComment((char *)"TOFF set:", toffset);
         //codegenGeneral(rhs);

         //toffset = savedToffset; // restore the offset
         //emitComment((char *)"TOFF set:", toffset);
         // increment can be + or -
         emitRM((char *)"LD", AC1, startoff, FP, (char *)"loop index");
         emitRM((char *)"LD", AC2, stopoff, FP, (char *)"stop value");
         emitRM((char *)"LD", AC, stepoff, FP, (char *)"step value");
         emitRO((char *)"SLT", AC, AC1, AC2, (char *)"Op <");
         emitRM((char *)"JNZ", AC, 1, PC, (char *)"Jump to loop body");

         skiploc = breakloc;                // save the old break statement return point!
         breakloc = emitSkip(1);            // addr of instr that jumps to end of loop
                                           // this is also the backpatch point

         // do body of loop
         codegenGeneral(current->child[2]);         // do body of loop

         
         // do increment stuff
         emitComment((char *)"Bottom of loop increment and jump");
         
         emitRM((char *)"LD", AC, startoff, FP, (char *)"Load index");
         emitRM((char *)"LD", AC2, stepoff, FP, (char *)"Load step");
         emitRO((char *)"ADD", AC, AC, AC2, (char *)"increment");
         emitRM((char *)"ST", AC, startoff, FP, (char *)"store back to index");

         // go to top of loop
         emitGotoAbs(currloc, (char *)"go to beginning of loop");
         backPatchAJumpToHere(breakloc, (char *)"Jump past loop [backpatch]");   // backpatch jump to end of loop

         breakloc = skiploc;              // restore for break statement
         emitComment((char *)"END LOOP");

         break;
      case CompoundK:
         savedToffset = toffset;
         toffset = current->size;   // recover the end of activation record

         emitComment((char *)"COMPOUND");
         emitComment((char *)"TOFF set:", toffset);
         codegenGeneral(lhs);  // process inits

         emitComment((char *)"Compound Body");
         codegenGeneral(rhs);  // process body
         
         toffset = savedToffset; // restore the offset
         emitComment((char *)"TOFF set:", toffset);
         emitComment((char *)"END COMPOUND");

         break;
      case ReturnK:
         emitComment((char *)"RETURN");
         if (lhs) {
            codegenExp(lhs);
            emitRM((char *)"LDA", RT, 0, AC, (char *)"Copy result to return register");
         }
         emitRM((char *)"LD", AC, RETURNOFFSET, FP, (char *)"Load return address");
         emitRM((char *)"LD", FP, OFPOFF, FP, (char *)"Adjust fp");
         emitGoto(0, AC, (char *)"Return");
         break;
      case BreakK:
         emitComment((char *)"BREAK");
         emitGotoAbs(breakloc, (char *)"break");
         //emitRM((char *)"LD", AC, RETURNOFFSET, FP, (char *)"Load return address");
         break;
      case RangeK:
         emitComment((char *)"RANGE");
         break;
   }
   return;
}

void codegenDecl(TreeNode *current) {
   //printf("Made it before NULL ===================\n");
   if (current == NULL) {
      return;
   }
   //printf("Made it into codegenDecl ===================\n");

   int offReg;
   TreeNode *lhs, *rhs;
   lhs = current->child[0];
   rhs = current->child[1];

   commentLineNum(current);

   switch(current->kind.decl) {
      case VarK:
         // Lots to do here
         if (current->isArray) {
            switch (current->varKind) {
               case Local:
                  emitRM((char *)"LDC", AC, current->size-1, 6, (char *)"load size of array", current->attr.name);
                  emitRM((char *)"ST", AC, current->offset+1, offsetRegister(current->varKind),(char *)"save size of array", current->attr.name);
                  break;
               case LocalStatic:
               case Parameter:
               case Global:
                  // do nothing here
                  break;
               case None:
                  // Error Condition
                  break;
            }

            // ARRAY VALUE initialization
            if (current->child[0]) {
               codegenExp(current->child[0]);
               emitRM((char *)"LDA", AC1, current->offset, offsetRegister(current->varKind), (char *)"address of lhs");
               emitRM((char *)"LD", AC2, 1, AC, (char *)"size of rhs");
               emitRM((char *)"LD", AC3, 1, AC1, (char *)"size of lhs");
               emitRO((char *)"SWP", AC2, AC3, 6, (char *)"pick smallest size");
               emitRO((char *)"MOV", AC1, AC, AC2, (char *)"array op =");
            }
         }
         else { // !current->isArray
            // SCALAR VALUE initialization
            if (current->child[0]) {
               switch (current->varKind) {
                  case Local:
                     // compute rhs -> AC;
                     codegenExp(current->child[0]);

                     // save it
                     emitRM((char *)"ST", AC, current->offset, FP, (char *)"Store variable", current->attr.name);
               case LocalStatic:
               case Parameter:
               case Global:
                  // do nothing here
                  break;
               case None:
                  ///Error condition!!!
                  break;
               }
            }
         }
         break;
      case FuncK:
         //printf("Made it into FuncK =============================================\n");
         if (current->lineno == -1) {     // These are the library functions we just added
            codegenLibraryFun(current);
         }
         else {
            codegenFun(current);
         }
         break;
      case ParamK:
         // IMPORTANT: no instructions need to be allocated for parameters here
         break;
   }
   return;
}

void codegenExp(TreeNode * current) {
   if (current == NULL) {
      //printf("Node is null!!\n");
      return;
   }

   //printf("Made it into codegenExp\n");

   int offReg;
   TreeNode *lhs, *rhs;
   lhs = current->child[0];
   rhs = current->child[1];

   switch(current->kind.exp) {
      case ConstantK:
         //printf("ConstantK ==================\n");
         switch (current->type) {
            case Boolean:
               emitRM((char *)"LDC", AC, current->attr.value, 6, (char *)"Load Boolean constant");
               break;
            case Integer:
               emitRM((char *)"LDC", AC, current->attr.value, 6, (char *)"Load integer constant");
               break;
            case Char:
               if (current->isArray) {
                  emitStrLit(current->offset, current->attr.string);
                  //emitRM((char *)"LDA", AD, current->offset, 0, (char *)"Load address of char array");
               }
               else {
                  emitRM((char *)"LDC", AC, current->attr.cvalue, 6, (char *)"Load char constant");
               }
               break;
            case Void:
               //printf("Case Void ==================\n");
               break;
         }
         break;
      case AssignK:
         //printf("AssignK ==================\n");
         //printf("AssignK\n");

         if (lhs == NULL) {
            //printf("shiddd\n");
            return;
         }
         //printf("yooooo\n");
         TreeNode *var, *idx;
         var = lhs->child[0];
         idx = lhs->child[1];

         if (lhs->attr.op == '[') {
            codegenExp(idx);

            if (rhs) {  // Will need to add to this later
               emitRM((char *)"ST", AC, toffset, FP, (char *)"Push index"); // These will need different values
               toffset--;
               emitComment((char *)"TOFF dec:", toffset);

               codegenExp(rhs);
               toffset++;
               emitComment((char *)"TOFF inc:", toffset);
               emitRM((char *)"LD", AC1, toffset, FP, (char *)"Pop index");    // These will need different values
            }

            switch(var->varKind) {
               case LocalStatic: // No break on purpose
               case Global:
                  emitRM((char *)"LDA", AC2, var->offset, GP, (char *)"Load address of base of array", var->attr.name);
                  break;
               case Local:
                  emitRM((char *)"LDA", AC2, var->offset, FP, (char *)"Load address of base of array", var->attr.name);
                  break;
               case Parameter:
                  emitRM((char *)"LD", AC2, var->offset, FP, (char *)"Load address of base of array", var->attr.name);
                  break;
            }
           
            if (rhs) {
               emitRO((char *)"SUB", AC2, AC2, AC1, (char *)"Compute offset of value");
               //if (var->isArray && var->varKind != Parameter) {
               //   emitRM((char *)"ST", AC, 0, AC2, (char *)"Store variable", var->attr.name);
               //}
            }
            else {
               emitRO((char *)"SUB", AC2, AC2, AC, (char *)"Compute offset of value");
            }


            /* 
            if (current->isArray && current->varKind == Parameter) {
               // Will add something similar to what comes next. Will also be a SUB
               emitRO((char *)"SUB", AC2, AC2, AC1, (char *)"Compute offset of value");
               emitRM((char *)"ST", AC, 0, AC2, (char *)"Store variable", var->attr.name);
            }
            else {
               emitRO((char *)"SUB", AC2, AC2, AC1, (char *)"Compute offset of value");
            }
            */
            switch(current->attr.op) {
               case '=':
                  emitRM((char *)"ST", AC, 0, AC2, (char *)"Store variable", var->attr.name);
                  break;
               case INC:
                  emitRM((char *)"LD", AC, 0, AC2, (char *)"load lhs variable", var->attr.name); // These will need different values
                  emitRM((char *)"LDA", AC, 1, AC, (char *)"increment value of", var->attr.name);
                  emitRM((char *)"ST", AC, 0, AC2, (char *)"Store variable", var->attr.name);
                  break;
               case DEC:
                  emitRM((char *)"LD", AC, 0, AC2, (char *)"load lhs variable", var->attr.name); // These will need different values
                  emitRM((char *)"LDA", AC, -1, AC, (char *)"decrement value of", var->attr.name);
                  emitRM((char *)"ST", AC, 0, AC2, (char *)"Store variable", var->attr.name);
                  break;
               case ADDASS:
                  emitRM((char *)"LD", AC1, 0, AC2, (char *)"load lhs variable", var->attr.name); // These will need different values
                  emitRO((char *)"ADD", AC, AC1, AC, (char *)"op +=");
                  emitRM((char *)"ST", AC, 0, AC2, (char *)"Store variable", var->attr.name);
                  break;
               case DIVASS:
                  emitRM((char *)"LD", AC1, 0, AC2, (char *)"load lhs variable", var->attr.name); // These will need different values
                  emitRO((char *)"DIV", AC, AC1, AC, (char *)"op /=");
                  emitRM((char *)"ST", AC, 0, AC2, (char *)"Store variable", var->attr.name);
                  break;
               case MULASS:
                  emitRM((char *)"LD", AC1, 0, AC2, (char *)"load lhs variable", var->attr.name); // These will need different values
                  emitRO((char *)"MUL", AC, AC1, AC, (char *)"op *=");
                  emitRM((char *)"ST", AC, 0, AC2, (char *)"Store variable", var->attr.name);
                  break;
               case SUBASS:
                  emitRM((char *)"LD", AC1, 0, AC2, (char *)"load lhs variable", var->attr.name); // These will need different values
                  emitRO((char *)"SUB", AC, AC1, AC, (char *)"op -=");
                  emitRM((char *)"ST", AC, 0, AC2, (char *)"Store variable", var->attr.name);
                  break;
               case '[':
                  emitRO((char *)"SUB", AC, AC1, AC, (char *)"compute location from index");
                  emitRM((char *)"ST", AC, 0, AC2, (char *)"Store variable", var->attr.name);
                  //printf("AAAAAAAA ===================\n");
                  break;
               case CHSIGN:
                  emitRM((char *)"LD", AC1, 0, AC2, (char *)"load lhs variable", var->attr.name); // These will need different values
                  emitRO((char *)"SUB", AC, AC1, AC, (char *)"op -");
                  emitRM((char *)"ST", AC, 0, AC2, (char *)"Store variable", var->attr.name);
                  break;
            }

            //emitRM((char *)"LCD", RT, 0, 6, (char *)"Set value to 0");
            // stuff
            // handle as an array
            
         }
         else {
            offReg = offsetRegister(lhs->varKind); // Was current->varKind, but slides say otherwise

            if (rhs) {
               codegenExp(rhs); // DO this to get Load integer constant
            }

            // Lots of cases will use this
            switch(current->attr.op) {
               case '=':
                  if (lhs->isArray) {
                     // Handle this in some way, but not like below
                     //emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name); 
                  }
                  else {
                     emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
                  }
                  break;
               case ADDASS:
                  emitRM((char *)"LD", AC1, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
                  emitRO((char *)"ADD", AC, AC1, AC, (char *)"op +=");
                  emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
                  break;
               case SUBASS:
                  emitRM((char *)"LD", AC1, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
                  emitRO((char *)"SUB", AC, AC1, AC, (char *)"op -=");
                  emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
                  break;
               case MULASS:
                  emitRM((char *)"LD", AC1, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
                  emitRO((char *)"MUL", AC, AC1, AC, (char *)"op *=");
                  emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
                  break;
               case DIVASS:
                  emitRM((char *)"LD", AC1, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
                  emitRO((char *)"DIV", AC, AC1, AC, (char *)"op /=");
                  emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
                  break;
               case DEC:
                  emitRM((char *)"LD", AC, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
                  emitRM((char *)"LDA", AC, -1, AC, (char *)"decrement value of", lhs->attr.name);
                  emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
                  break;
               case INC:
                  emitRM((char *)"LD", AC, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
                  emitRM((char *)"LDA", AC, 1, AC, (char *)"increment value of", lhs->attr.name);
                  emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
                  break;
               case BREAK:
                  //printf("Eeughhh =====\n");
                  break;
               case '-':
                  emitRM((char *)"LD", AC, lhs->offset, offReg, (char *)"load lhs variable", lhs->attr.name);
                  emitRM((char *)"LDA", AC, 1, AC, (char *)"increment value of", lhs->attr.name);
                  emitRM((char *)"ST", AC, lhs->offset, offReg, (char *)"Store variable", lhs->attr.name);
                  break;
            }
            //emitRM((char *)"ST", AC, current->offset, FP, (char *)"Store variable", lhs->attr.name);
         }
         break;
      case IdK:
         //printf("IdK ==================\n");
         offReg = offsetRegister(current->varKind);

         if (!current->isArray) {
            emitRM((char *)"LD", AC, current->offset, offReg, (char *)"Load variable", current->attr.name);
         }
         else {
            //codegenExp(lhs);
            if (current->varKind == Parameter) {
               //printf("Made it here ============================================\n");
               emitRM((char *)"LD", AC, current->offset, offReg, (char *)"Load address of base of array", current->attr.name);
            }
            else {
               //printf("Made it to else ============================================\n");
               emitRM((char *)"LDA", AC, current->offset, offReg, (char *)"Load address of base of array", current->attr.name);
            }
            
         }
         /*
         if (current->isArray) {
            //printf("is array ========================\n");
            if (current->varKind != Parameter) {
               emitRM((char *)"ST", AC, current->offset, offReg, (char *)"Store variable", current->attr.name);
            }
         }
         else {
            if (current->varKind == Parameter) {
               emitRM((char *)"LD", AC, current->offset, offReg, (char *)"Load parameter", current->attr.name);
            }
            else {
               if (lhs == NULL) {
                  emitRM((char *)"LD", AC, current->offset, offReg, (char *)"Load variable", current->attr.name);
               }
               else {
                  codegenExp(lhs);
                  emitRM((char *)"LD", AC, current->offset, offReg, (char *)"Load variable", current->attr.name);
               }
            }
         }
         
         offReg = offsetRegister(current->varKind);
         */

         break;
      case CallK:
         // printf("CallK ==================\n");
         TreeNode *funcNode, *param;  // We want to look up the name of the function
         int ghostFrame, callLoc;

         funcNode = ((TreeNode *)(globals->lookup(current->attr.name)));

         emitComment((char *)"CALL", current->attr.name);
         callLoc = funcNode->offset;
         ghostFrame = toffset;
         emitRM((char *)"ST", FP, ghostFrame, FP, (char *)"Store fp in ghost frame for", current->attr.name);

         toffset--;
         emitComment((char *)"TOFF dec:", toffset);

         toffset--;
         emitComment((char *)"TOFF dec:", toffset); 

         param = current->child[0];
         
         {
            int i;
            char buffer[16];

            i = 1;
            while (param) {
               sprintf(buffer, "%d", i); // Turning the number i into a string that is stored in buffer
               emitComment((char *)"Param", buffer);
               codegenExp(param);
               emitRM((char *)"ST", AC, toffset, FP, (char *)"Push parameter");
               toffset--;
               emitComment((char *)"TOFF dec:", toffset);
               param = param->sibling;
               i++;
            }
         }

         emitComment((char *)"Param end", current->attr.name);

         emitRM((char *)"LDA", FP, ghostFrame, FP, (char *)"Ghost frame becomes new active frame");
         emitRM((char *)"LDA", AC, 1, PC, (char *)"Return address in ac");
         
         emitGotoAbs(callLoc, (char *)"CALL", current->attr.name);
         emitRM((char *)"LDA", AC, 0, RT, (char *)"Save the result in ac");
         emitComment((char *)"Call end", current->attr.name);

         toffset = ghostFrame;
         emitComment((char *)"TOFF set:", toffset);;

         break;
      case OpK:
         codegenExp(lhs);

         //printf("OpK ==================\n");
         if (rhs) {
            emitRM((char *)"ST", AC, toffset, FP, (char *)"Push left side");
            toffset--;
            emitComment((char *)"TOFF dec:", toffset);

            codegenExp(rhs);
            toffset++;
            emitComment((char *)"TOFF inc:", toffset);
            emitRM((char *)"LD", AC1, toffset, FP, (char *)"Pop left into ac1");

            switch(current->attr.op) {
               case '[':
                  emitRO((char *)"SUB", AC, AC1, AC, (char *)"compute location from index");
                  emitRM((char *)"LD", AC, 0, AC, (char *)"Load array element");
                  break;
               case '+':
                  emitRO((char *)"ADD", AC, AC1, AC, (char *)"Op +");
                  break;
               case '-':
                  emitRO((char *)"SUB", AC, AC1, AC, (char *)"Op -");
                  break;
               case '*':
                  emitRO((char *)"MUL", AC, AC1, AC, (char *)"Op *");
                  break;
               case '%':
                  emitRO((char *)"MOD", AC, AC1, AC, (char *)"Op %");
                  break;
               case '/':
                  emitRO((char *)"DIV", AC, AC1, AC, (char *)"Op /");
                  break;
               case MIN:
                  emitRO((char *)"SWP", AC, AC1, AC, (char *)"Op :<:");
                  break;
               case MAX:
                  emitRO((char *)"SWP", AC1, AC, AC, (char *)"Op :>:");
                  break;
               case LEQ:
                  emitRO((char *)"TLE", AC, AC1, AC, (char *)"Op <=");
                  break;
               case GEQ:
                  emitRO((char *)"TGE", AC, AC1, AC, (char *)"Op >=");
                  break;
               case EQ:
                  emitRO((char *)"TEQ", AC, AC1, AC, (char *)"Op ==");
                  break;
               case NEQ:
                  emitRO((char *)"TNE", AC, AC1, AC, (char *)"Op !=");
                  break;
               case '<':
                  emitRO((char *)"TLT", AC, AC1, AC, (char *)"Op <");
                  break;
               case '>':
                  emitRO((char *)"TGT", AC, AC1, AC, (char *)"Op >");
                  break;
               case AND:
                  emitRO((char *)"AND", AC, AC1, AC, (char *)"Op AND");
                  break;
               case OR:
                  emitRO((char *)"OR", AC, AC1, AC, (char *)"Op OR");
                  break;
            }
         } else {
            switch (current->attr.op) {
               case CHSIGN:
                  //emitRM((char *)"LDC", AC, 2, 6, (char *)"Load integer constant");
                  emitRO((char *)"NEG", AC, AC, AC, (char *)"Op unary -");   
                  break;
               case SIZEOF:
                  //emitRM((char *)"LDA", AC, -3, FP, (char *)"Load address of base of array", lhs->attr.name);
                  emitRM((char *)"LD", AC, FP, AC, (char *)"Load array size");
                  break;
               case NOT:
                  //emitRM((char *)"LDC", AC, FP, AC3, (char *)"Load Boolean constant");
                  emitRM((char *)"LDC", AC1, FP, AC3, (char *)"Load 1");
                  emitRO((char *)"XOR", AC, AC, AC1, (char *)"Op XOR to get logical not");
                  break;
               case '?':
                  //emitRM((char *)"LDC", AC, 9, AC3, (char *)"Load integer constant");
                  emitRO((char *)"RND", AC, AC, AC3, (char *)"Op ?");
                  break;
            }
         }

         // More code here
         break;
      default:
         emitComment((char *)"Default case for codegenExp()");
         break;
   }

   return;
}

