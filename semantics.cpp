#include "treeNodes.h"
#include "treeUtils.h"
#include "semantics.h"
#include "symbolTable.h"
#include <string.h>
#include "parser.tab.h"

// Memory offsets
static int goffset = 0;  // Top of global space
static int foffset = 0;  // Top of local space
static int varCounter = 0;
static bool newScope = false;
// // // // // // // // // // // // // // // // //
//
// Symbol Table Functions
//
// debug(bool state) - will set the debug flag
// depth() - will return the number of scopes in the symbol table
// enter(std::string name) - Used when entering a scope
// leave() - Leaves a scope. Not allowed to leave global!
// lookup(std::string sym) - Lookup a symbol anywhere in the stack of scopes. Returns NULL if not found, or void pointer with the symbol
// lookupGlobal(std::string sym) - Lookup a symbol in the global scope. Returns NULL if not found, or a void pointer with the symbol
// insert(std::string sym, void *ptr) - Insert a symbol into the most recent scope. Returns true if successful, or false if symbol is in the scope already
// insertGlobal(std::string sym, void *ptr) - Insert a symbol into the global scope. Returns true if successful, or false if already in the global scope
// applyToAll(void (*action)(std::string, void *)) - Apply function to each simple in the scope
// applyToAllGlobal(void (*action)(std::string, void *)) - Apply function to each simple in the global scope
//
// // // // // // // // // // // // // // // // //

// Remember! Terminals are in the child nodes and do not get traversed

void treeTraverse(TreeNode *current, SymbolTable *symtab);
TreeNode *loadIOLib(TreeNode *syntree);

bool insertCheck(TreeNode *current, SymbolTable *symtab) {
   if (!symtab->insert(current->attr.name, current)) {
      printf("Error: Symbol is already defined.\n");
      return false;
   }

   return true;
}

void treeTraverseStmt(TreeNode *current, SymbolTable *symtab)
{
   TreeNode *c0, *c1, *c2, *temp;
   c0 = current->child[0];
   c1 = current->child[1];
   c2 = current->child[2];
   int prevOffset = foffset;
   //bool isCompound = false;

   switch (current->kind.stmt) {
      case IfK:
         //printf("IfK: %s\n\n", current->attr.name);

         treeTraverse(c0, symtab);
         treeTraverse(c1, symtab);
         treeTraverse(c2, symtab);

         break;

      case WhileK:
         //printf("WhileK: %s\n\n", current->attr.name);
         //symtab->enter((char *) "whileStmt");

         // Only will ever have two children
         treeTraverse(c0, symtab);
         treeTraverse(c1, symtab);
         break;

      case CompoundK:
         //printf("CompoundK\n");
         // Pass the type up the tree
         //current->type = c0->type;

         symtab->enter((char *) "compoundStmt");
         prevOffset = foffset;

         treeTraverse(c0, symtab);
         current->size = foffset;

         treeTraverse(c1, symtab);
         foffset = prevOffset;

         symtab->leave();

         /*
         symtab->enter((char *) "compoundStmt");
          
         if (newScope) {
            // Remember current offset
            prevOffset = foffset;

            // Left side of compound
            treeTraverse(c0, symtab);

            current->size = foffset;

            // Right side of compound
            treeTraverse(c1, symtab);

            //remember the current offset
            prevOffset = foffset;
         }
         else {
            // Left side of compound
            treeTraverse(c0, symtab);

            current->size = foffset;

            // Right side of compound
            treeTraverse(c1, symtab);
         }
         
         symtab->leave();
         */
         break;

      case ForK:
         //printf("ForK: %s\n\n", current->attr.name);
         // Remember the current offset
         symtab->enter((char *) "compoundStmt");
         prevOffset = foffset;

         // Traverse child 0 tree
         treeTraverse(c0, symtab);

         foffset -= 2; //Make space for the for loop var


         current->size = foffset;

         treeTraverse(c1, symtab);
         treeTraverse(c2, symtab);

         // Restore offset at the end
         prevOffset = foffset;
         symtab->leave();

         break;

      case RangeK:
         //printf("RangeK: %s\n\n", current->attr.name);
         treeTraverse(c0, symtab);
         treeTraverse(c1, symtab);
         treeTraverse(c2, symtab);
         break;

      case ReturnK:
         if (c0 != NULL) {
            treeTraverse(c0, symtab); 
         }
         //printf("ReturnK: %s\n\n", current->attr.name);
         break;

      default:
         //printf("Unknown statement node kind: %d", current->kind.stmt);
         treeTraverse(c0, symtab);
         treeTraverse(c1, symtab);
         treeTraverse(c2, symtab);
         break;
   }

   return;
}

void treeTraverseDecl(TreeNode *current, SymbolTable *symtab)
{
   TreeNode *c0, *c1, *c2, *temp;
   c0 = current->child[0];
   c1 = current->child[1];
   c2 = current->child[2];
   int prevOffset = foffset;
   bool isCompound = false;

   switch (current->kind.decl) {
      case VarK:
         //printf("VarK: %s\n", current->attr.name);
         
         // Push symbol onto the stack
         if (current->child[0] != NULL)
         {
            current->isAssigned = true;
            treeTraverse(c0, symtab);
         }

         insertCheck(current, symtab);

         if (symtab->depth()==1) { //if this is global
            //set the varKind to Global for VarK
            current->offset = goffset;
            goffset -= current->size;
            current->varKind = Global;

            symtab->insertGlobal(current->attr.name, (void *)current);
         }
         else if(current->isStatic) {
            current->offset = goffset;
            goffset -= current->size;
            current->varKind = LocalStatic;

            symtab->insertGlobal(current->attr.name, (void *)current);            
         }
         else { //treat it as normal
            current->offset = foffset;
            foffset -= current->size;
            current->varKind = Local;

            symtab->insert(current->attr.name, (void *)current);
            //if(current->isArray) { current->offset--; }
         }
         if(current->isArray) { current->offset--; }
         break;
      case FuncK:
         //printf("FuncK: %s\n\n", current->attr.name);

         if (current->attr.name == NULL)
         {
            printf("FuncK: current->attr.name not set\n");
            break;
         }

         current->varKind = Global;
         foffset = -2; // Only time this is done

         //insertCheck(current, symtab);

         // any time there are curly brackets, enter a new scope
         symtab->enter(current->attr.name);

         symtab->insertGlobal(current->attr.name, (void *)current);

         treeTraverse(c0, symtab);
         current->size = foffset;
         treeTraverse(c1, symtab);
         treeTraverse(c2, symtab);

         symtab->leave();

         break;
      case ParamK:
         //printf("ParamK: %s\n\n", current->attr.name);
         current->varKind = Parameter;

         if (insertCheck(current, symtab)) {
            if (symtab->depth() == 1) {
               current->offset = goffset;
               goffset -= current->size;
               current->varKind = Global;
            }
            else if (current->isStatic) {
               current->offset = goffset;
               goffset -= current->size;
               current->varKind = LocalStatic;
               //symtab->insertGlobal(current->attr.name, (void *)current); 
               { // Create a new scope in case newName is already being used
                    char *newName;

                    newName = new char [strlen(current->attr.name)+10];
                    sprintf(newName, "%s-%d", current->attr.name, ++varCounter);
                    symtab->insertGlobal(newName, current);

                    delete [] newName;
                }
               //symtab->insertGlobal with a unique name. Keep a stat int variable and append it to the end of the name
            }
            else { //treat it as normal
               current->offset = foffset; // Was foffset, changed to test something
               foffset -= current->size;
               //if(current->isArray) { current->offset--; }
            }
         }
         break;
      default:
         //printf("Unknown declaration node kind: %d", current->kind.decl);
         treeTraverse(c0, symtab);
         treeTraverse(c1, symtab);
         treeTraverse(c2, symtab);
         break;
   }

   return;
}

void treeTraverseExp(TreeNode *current, SymbolTable *symtab)
{
   TreeNode *c0, *c1, *c2, *temp;
   c0 = current->child[0];
   c1 = current->child[1];
   c2 = current->child[2];
   int prevOffset = foffset;
   bool isCompound = false;

   switch (current->kind.exp) {
      case AssignK:      
 
      // No break on purpose
      case OpK:
         //printf("AssignK/OpK: %s\n", current->attr.name);

         /*
         if (c0 == NULL) {
            printf("No left child!\n"); 
         }
         else {
            //printf("Left child found %s\n", c0->attr.name);
         }*/

         // Deal with left child
         //printf("OpK: %s\n", current->attr.name);

         treeTraverse(c0, symtab);

         //treeTraverse(c1, symtab);
         
         // Deal with self
         current->type = Integer;
         switch (current->attr.op){
            //case SIZEOF:
            //case CHSIGN:
            case '=':
            case '[':
               current->type = c0->type;
               break;

            case AND:
            case OR:
            case EQ:
            case NEQ:
            case LEQ:
            case '<':
            case GEQ:
            case '>':
            case NOT:
               current->type = Boolean;
               break;

            default:
               break;
            }

         // Deal with right child
         treeTraverse(c1, symtab);

         // new scope for integer: Any value < LASTOP
         /*
         else if (current->type == Integer) {
            // new scope for integer: Any value < LASTOP
            if(current->attr.value < 259){
               symtab->enter(current->attr.name); // Enter a new scope

               treeTraverse(c0, symtab);
               current->size = foffset;
               treeTraverse(c1, symtab);
               treeTraverse(c2, symtab);

               symtab->leave();
            }
            else
            {
               treeTraverse(c0, symtab);
               current->size = foffset;
               treeTraverse(c1, symtab);
               treeTraverse(c2, symtab);
            }

         }

         if (c0->isArray == true) {
            // new scope for =, [ regardless of type as long as it is an array
         }
         */
         break;
      case ConstantK:
         //printf("ConstantK: %s\n", current->attr.name);

         // If an array, treat as a string
         if (current->type == Char && current->isArray) {
            current->varKind = Global;
            current->offset = goffset - 1;
            goffset -= current->size;
         }
         break;
      case IdK:
         //printf("IdK: %s\n", current->attr.name);
         //printf("Made it to IdK %s\n", current->attr.name);

         if ((temp = (TreeNode *)(symtab->lookup(current->attr.name)))) { // Lookup in the symbol table
            //printf("Found %s\n", current->attr.name);
            current->offset = temp->offset;
            current->type = temp->type;
            current->isStatic = temp->isStatic;
            current->isArray = temp->isArray;
            current->size = temp->size;
            current->varKind = temp->varKind;
         }
         break;
      case CallK:
         //printf("CallK: %s\n\n", current->attr.name);

         treeTraverse(current->child[0], symtab);
         if (temp = (TreeNode *)(symtab->lookup(current->attr.name))) {
            current->type = temp->type;
            current->size = temp->size;
         }
         break;
      default:
         //printf("Unknown expression node kind: %d", current->kind.exp);
         treeTraverse(current->child[0], symtab);
         treeTraverse(current->child[1], symtab);
         treeTraverse(current->child[2], symtab);
         break;
   }

   return;
}

void treeTraverse(TreeNode *current, SymbolTable *symtab) {
   // Check if node is NULL
   if (current == NULL) {
      return;
   }

   // Handle all node types
   switch (current->nodekind) {
      case DeclK:
         treeTraverseDecl(current, symtab);
         break;
      case StmtK:
         treeTraverseStmt(current, symtab);
         break;
      case ExpK:
         treeTraverseExp(current, symtab);
         break;
   }

   // Check for siblings
   if (current->sibling != NULL) {
      treeTraverse(current->sibling, symtab);
   }

   return;
}

TreeNode *loadIOLib(TreeNode *syntree) {
   TreeNode *input, *output, *param_output;
   TreeNode *inputb, *outputb, *param_outputb;
   TreeNode *inputc, *outputc, *param_outputc;
   TreeNode *outnl;
  
 
   input = newDeclNode(DeclKind::FuncK, ExpType::Integer);           // #1
   input->lineno = -1;
   input->attr.name = strdup("input");
   input->type = Integer;
   input->varKind = Global;

   param_output = newDeclNode(DeclKind::ParamK, ExpType::Void);       // #3
   param_output->lineno = -1;
   param_output->attr.name = strdup("*dummy*");
   param_output->type = Integer;
   param_output->varKind = Parameter;
   param_output->offset = -2;

   output = newDeclNode(DeclKind::FuncK, ExpType::Void);             // #2
   output->lineno = -1;
   output->attr.name = strdup("output");
   output->type = Void;
   output->child[0] = param_output;
   output->varKind = Global;

   inputb = newDeclNode(DeclKind::FuncK, ExpType::Boolean);          // #4
   inputb->lineno = -1;
   inputb->attr.name = strdup("inputb");
   inputb->type = Boolean;
   inputb->varKind = Global;

   param_outputb = newDeclNode(DeclKind::ParamK, ExpType::Void);     // #6
   param_outputb->lineno = -1;
   param_outputb->attr.name = strdup("*dummy*");
   param_outputb->type = Boolean;
   param_outputb->varKind = Parameter;
   param_outputb->offset = -2;

   outputb = newDeclNode(DeclKind::FuncK, ExpType::Void);            // #5
   outputb->lineno = -1;
   outputb->attr.name = strdup("outputb");
   outputb->type = Void;
   outputb->child[0] = param_outputb;
   outputb->varKind = Global;

   inputc = newDeclNode(DeclKind::FuncK, ExpType::Boolean);          // #7
   inputc->lineno = -1;
   inputc->attr.name = strdup("inputc");
   inputc->type = Char;
   inputc->varKind = Global;

   param_outputc = newDeclNode(DeclKind::ParamK, ExpType::Void);     // #9
   param_outputc->lineno = -1;
   param_outputc->attr.name = strdup("*dummy*");
   param_outputc->type = Char;
   param_outputc->varKind = Parameter;
   param_outputc->offset = -2;
   //param_outputc->size = 1;

   outputc = newDeclNode(DeclKind::FuncK, ExpType::Void);            // #8
   outputc->lineno = -1;
   outputc->attr.name = strdup("outputc");
   outputc->type = Void;
   outputc->child[0] = param_outputc;
   outputc->varKind = Global;

   outnl = newDeclNode(DeclKind::FuncK, ExpType::Void);              // #10
   outnl->lineno = -1;
   outnl->attr.name = strdup("outnl");
   //outnl->type = Integer;
   outnl->child[0] = NULL;
   outnl->varKind = Global;
   

   input->sibling = output;
   output->sibling = inputb;
   inputb->sibling = outputb;
   outputb->sibling = inputc;
   inputc->sibling = outputc;
   outputc->sibling = outnl;
   outnl->sibling = syntree;  // add in the tree we were given

   return input;
}

TreeNode *semanticAnalysis(TreeNode *syntree,          // pass in and return an annotated syntax tree
                           bool shareCompoundSpaceIn,   // SPECIAL OPTION: make compound after a function share scope
                           bool noDuplicateUndefsIn,    // SPECIAL OPTION: no duplicate undefines
                           SymbolTable *symtabX,       // pass in and return the symbol table
                           int &globalOffset            // return the offset past the globals
                           ) {
   //goffset = globalOffset; // Was enabled, but was replaced and moved 

   // Will need loadIOLibs
   syntree = loadIOLib(syntree);

   // traverse the tree
   treeTraverse(syntree, symtabX);

   globalOffset = goffset;

   // return the updated syntree
   return syntree;
}

/*
For ExpK, IdK: Look up the type in the symtab
   if ((tmp = (TreeNode *)(symtab->lookup(current->attr.name)))) {
      current->type = tmp->type;
      current->isStatic = tmp->isStatic
      current->isArray
      current->size
      current->foffset
   }


For DeclK, VarK & ParamK:
   if (symtab->depth()==1) { //if this is global
      //set the varKind to Global for VarK
      current->offset = goffset;
      goffset -= current->size;
   }
   else if(...) {
      current->offset = goffset;
      goffset -= current->size;
      //symtab->insertGlobal with a unique name. Keep a stat int variable and append it to the end of the name
   }
   else { //treat it as normal
      current->offset = foffset;
      foffset -= current->size;
      if(current->isArray) { current->offset--; }
   }

For StmtK, CompoundK
   if new scope
      remember current offset

For StmtK, ForK
   remember the current offset and restore it at the end
   traverse child 0 tree
*/

