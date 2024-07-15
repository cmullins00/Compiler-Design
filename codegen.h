#ifndef CODEGEN_H
#define CODEGEN_H

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "treeNodes.h"
//#include "treeUtils.h"
//#include "symbolTable.h"
//#include "emitcode.h"
//using namespace std;

void codegen(FILE *codeIn,          // where the code should be written
             char *srcFile,         // name of file compiled
             TreeNode *syntaxTree,  // tree to process
	     SymbolTable *globalsIn,     // globals so function info can be found
	     int globalOffset,      // size of the global frame
             bool linenumFlagIn);   // comment with line numbers

void codegenHeader(char *srcFile);
int offsetRegister(VarKind v);
void codegenStmt(TreeNode *current);
void codegenDecl(TreeNode *current);
void codegenExp(TreeNode *current);
void codegenGeneral(TreeNode *current);
void commentLineNum(TreeNode *current);
void codegenLibraryFun(TreeNode *current);
void codegenFun(TreeNode *current);
void initGlobalArraySizes();
void codegenInit(int initJump, int globalOffset);
void initAGlobalSymbol(std::string sym, void *ptr);

#endif
