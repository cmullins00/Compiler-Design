#ifndef _UTIL_H_
#define _UTIL_H_
#include "treeNodes.h"
#include "scanType.h"
#include <string.h>
#include <stdlib.h>

// lots of these save the TokenData block so line number and yytext are saved
TreeNode *cloneNode(TreeNode *current);

// Create a new DeclNode
TreeNode *newDeclNode(DeclKind kind, ExpType type, TokenData *token, TreeNode *c0, TreeNode *c1, TreeNode *c2) // save TokenData block!!
{
   TreeNode * newNode = (TreeNode*)malloc(sizeof(TreeNode));
   newNode->nodekind = NodeKind::DeclK;
   newNode->kind.decl = kind;
   newNode->size = 1;
   newNode->type = type;
   newNode->isArray = false;
   newNode->isUsed = false;

   // All children are initially NULL
   newNode->child[0] = c0;
   newNode->child[1] = c1;
   newNode->child[2] = c2;
   newNode->sibling = NULL;

   if (token != NULL) {
      newNode->lineno = (token->linenum) ? token->linenum : 1;
      newNode->attr.name = token->tokenstr;
   }

   return newNode;
}

// Create a new StmtNode
TreeNode *newStmtNode(StmtKind kind, TokenData *token, TreeNode *c0, TreeNode *c1, TreeNode *c2)
{
   TreeNode * newNode = (TreeNode*)malloc(sizeof(TreeNode));
   newNode->nodekind = NodeKind::StmtK;
   newNode->kind.stmt = kind;
   newNode->size = 1;
   newNode->lineno = token->linenum;
   newNode->attr.name = token->tokenstr;
   newNode->isArray = false;
   newNode->isUsed = false;

   // All children are initially NULL
   newNode->child[0] = c0;
   newNode->child[1] = c1;
   newNode->child[2] = c2;
   newNode->sibling = NULL;

   /* add compoundStmt, matched, unmatched, iterRange, returnStmt, breakStmt, */
   return newNode;
}

// Create a new ExpNode
TreeNode *newExpNode(ExpKind kind, TokenData *token, TreeNode *c0, TreeNode *c1, TreeNode *c2)
{
   TreeNode * newNode = (TreeNode*)malloc(sizeof(TreeNode));
   newNode->nodekind = NodeKind::ExpK;
   newNode->kind.exp = kind;
   newNode->size = 1;
   newNode->lineno = token->linenum;
   newNode->attr.op = token->tokenclass;
   newNode->attr.name = token->tokenstr;
   newNode->isArray = false;
   newNode->isUsed = false;

   // All children are initially NULL
   newNode->child[0] = c0;
   newNode->child[1] = c1;
   newNode->child[2] = c2;
   newNode->sibling = NULL;

   //newNode->attr.string = expTypeToStr(newNode->type, newNode->isArray, newNode->isStatic);
   /* in newExpNode:
      exp, simpleExp, andExp, unaryRelExp, relExp, minmaxExpn, sumExp, mulExp, nuaryExp, mutable, call, constant*/
   return newNode;
}

// Convert a token to a string
char *tokenToStr(int type);

// Convert an Exp to a string
char *expTypeToStr(ExpType type, bool isArray=false, bool isStatic=false);

// Print all tree nodes
void printTreeNode(FILE *out, TreeNode *syntaxTree, bool showExpType, bool showAllocation);

// Print the tree
void printTree(FILE *out, TreeNode *syntaxTree, bool showExpType, bool showAllocation);

char expTypeToStrBuffer[80];
char *expTypeToStr(ExpType type, bool isArray, bool isStatic)
{
   char * typeName;
   // The type needs to be recorded in parser.y

   switch(type)
   {
   case Void:
      typeName = (char *)"type void";
      break;
   case Integer:
      typeName = (char *)"type int";
      break;
   case Boolean:
      typeName = (char *)"type bool";
      break;
   case Char:
      typeName = (char *)"type char";
      break;
   case UndefinedType:
      typeName = (char *)"type undefined";
      break;
   default:
      printf("Default case");
      char *buffer;
      buffer = new char [80];
      sprintf(buffer, "invalid expType: %d", (int)type);
      return buffer;
      break;
   }

   sprintf(expTypeToStrBuffer, "%s%s%s", (isStatic ? "static " : ""), (isArray ? "array of " : ""), typeName);

   return strdup(expTypeToStrBuffer);
}

void printTreeRecursive(FILE *out, TreeNode *syntaxTree, bool showExpType, bool showAllocation, int depth, int siblingCount = 1)
{
   if (syntaxTree == NULL)
   {
      return;
   }

   // Draw enough ".  .  ." for this node, so we want to know what depth we are at.
   printTreeNode(out, syntaxTree, showExpType, showAllocation);
   fprintf(out, "\n");

   int q, i;
   for (q = 0; q < MAXCHILDREN; q++)
   {
      // draw .  .  .  depth

      if (syntaxTree->child[q] != NULL)
      {
         for (i = 0; i <= depth; i++)
         {
            printf(".   ");
         }

         // two spaces at the end on purpose
         fprintf(out, "Child: %d  ", q);

         printTreeRecursive(out, syntaxTree->child[q], showExpType, showAllocation, depth+1);
      }
   }

   TreeNode *sibling = syntaxTree->sibling;
   if (sibling)
   {
      // show  .  .  .  depth
      for(i = 0; i < depth; i++)
      {
         printf(".   ");
      }

      // again add two spaces at the end
      fprintf(out, "Sibling: %d  ", siblingCount);
      printTreeRecursive(out, sibling, showExpType, showAllocation, depth, siblingCount + 1);
   }

   return;
}

char *varKindToStr(int kind)
{
    switch (kind) {
    case None:
        return (char *)"None";
    case Local:
        return (char *)"Local";
    case Global:
        return (char *)"Global";
    case Parameter:
        return (char *)"Parameter";
    case LocalStatic:
        return (char *)"LocalStatic";
    default:
        return (char *)"unknown kind in varKindToStr";
    }
}

void showAllocation(FILE *out, TreeNode *tree)
{
   fprintf(out, "[mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);

   return;
}

// print a node without a newline
void printTreeNode(FILE *listing, TreeNode *tree, bool showExpType, bool showAllocation) 
{ 
   // print a declaration node
   if (tree->nodekind == DeclK) {
      switch (tree->kind.decl) {
         case VarK:
            printf("Var: %s ", tree->attr.name);
            printf("of %s", expTypeToStr(tree->type, tree->isArray, tree->isStatic));
            if (showAllocation) {
                printf(" [mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);
            }
            break;
         case FuncK:
            printf("Func: %s ", tree->attr.name);
            printf("returns %s", expTypeToStr(tree->type, tree->isArray, tree->isStatic));
            if (showAllocation) {
                printf(" [mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);
            }
            break;
         case ParamK:
            printf("Parm: %s ", tree->attr.name);
            printf("of %s", expTypeToStr(tree->type, tree->isArray, tree->isStatic));
            if (showAllocation) {
                printf(" [mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);
            }
            break;
         default:
            fprintf(listing, "Unknown declaration node kind: %d",
            tree->kind.decl);
            break;
      }
   }

   // print a statement node
   else if (tree->nodekind == StmtK) {
      switch (tree->kind.stmt) {
      case IfK:
         fprintf(listing, "If");
         break;
      case WhileK:
         fprintf(listing, "While");
         break;
      case CompoundK:
         fprintf(listing, "Compound");
            if (showAllocation) {
               printf(" [mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);
            }
         break;
      case ForK:
         fprintf(listing, "For");
         if (showAllocation) {
            printf(" [mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);
         }
         break;
      case RangeK:
         fprintf(listing, "Range");
         break;
      case ReturnK:
         fprintf(listing, "Return");
         break;
      case BreakK:
         fprintf(listing, "Break");
         break;
      default:
         fprintf(listing, "Unknown  statement node kind: %d",tree->kind.stmt);
         break;
      }
   }

   // print an expression node
   else if (tree->nodekind == ExpK) {
      switch (tree->kind.exp) {
      case AssignK:
         fprintf(listing, "Assign: %s", tokenToStr(tree->attr.op));
         break;
      case OpK:
         fprintf(listing, "Op: %s", tokenToStr(tree->attr.op));
         break;
      case ConstantK:
         switch (tree->type) {
            case Boolean:
               fprintf(listing, "Const %s", (tree->attr.value) ?  "true" : "false");
               break;
            case Integer:
               fprintf(listing, "Const %d", tree->attr.value);
               break;
            case Char:
               if (tree->isArray) {
                  fprintf(listing, "Const ");
                  printf("\"");
                  for (int i=0; i < tree->size-1; i++) {
                     printf("%c", tree->attr.string[i]);
                  }
                  printf("\"");
               }
               else {
                  if (tree->attr.cvalue == '\\')
                     fprintf(listing, "Const '\n'");
                  else
                     fprintf(listing, "Const '%c'", tree->attr.cvalue);
               }
               break;
            case Void:
            case UndefinedType:
               fprintf(listing, "SYSTEM ERROR: parse tree contains invalid type for constant: %s\n", expTypeToStr(tree->type));
         }
         break;
      case IdK:
         fprintf(listing, "Id: %s", tree->attr.name);
         break;
      case CallK:
         fprintf(listing, "Call: %s", tree->attr.name);
         break;
      default:
         fprintf(listing, "Unknown expression node kind: %d", tree->kind.exp);
         break;
      }

      if (showExpType){
         fprintf(listing, " of %s", expTypeToStr(tree->type, tree->isArray, tree->isStatic));
      }

      if (showAllocation){
         if (tree->kind.exp == IdK || tree->kind.exp == ConstantK && tree->type == Char && tree->isArray){
            printf(" [mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);
         }
      }
   }
   else
   { 
      fprintf(listing, "Unknown class of node: %d", tree->nodekind);
   }

   fprintf(listing, " [line: %d]", tree->lineno);
}

void printTree(FILE *out, TreeNode *syntaxTree, bool showExpType, bool showAllocation)
{
   if (syntaxTree == NULL)
   {
      fprintf(out, "NULL\n");
      return;
   }

   //printf("About to print tree\n");

   printTreeRecursive(out, syntaxTree, showExpType, showAllocation, 0);

   return;
}
#endif
