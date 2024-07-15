%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include "treeUtils.h"
#include "scanType.h"
#include "semantics.h"
#include "symbolTable.h"
#include "emitcode.h"
#include "codegen.h"
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

int numErrors = 0;
int numWarnings = 0;
extern int line;
extern int yylex();

void yyerror(const char *msg);

SymbolTable *symtab;
//symtab = new SymbolTable();
//symtab->debug(false);

TreeNode *addSibling(TreeNode *t, TreeNode *s)
{
   // make sure s is not null. If it is this, then there is a major error. Exit the program!
   if (s == NULL)
   {
      printf("TreeNode addSibling: TreeNode *s is null! Exiting the program.\n");
      exit(-1);
   }

   // Make sure t is not null. If it is, just return s
   if (t == NULL)
   {
      return s;
   }

   TreeNode *temp = t;
   // Look down t's sibling list until you fin with sibling = null (end of the list) and add s there
   while (temp->sibling != NULL)
   {
      temp = temp->sibling;
   }

   temp->sibling = s;

   return t;
}

void setType(TreeNode *t, ExpType type, bool isStatic)
{
   while (t)
   {
      // Set t->type and t->isStatic
      t->type = type;
      t->isStatic = isStatic;

      // Move to next sibling
      t = t->sibling;
   }
}

void printToken(TokenData myData, string tokenName, int type = 0) {
   cout << "Line: " << myData.linenum << " Type: " << tokenName;
   if(type==0)
     cout << " Token: " << myData.tokenstr;
   if(type==1)
     cout << " Token: " << myData.nvalue;
   if(type==2)
     cout << " Token: " << myData.cvalue;
   cout << endl;
}

TreeNode * syntaxTree;

%}

%union
{
   TokenData *tinfo;
   TreeNode *tree;
   ExpType type;  // for passing type spec up the tree
}
%type <type> typeSpec

%type <tree> program
%type <tree> precomList
%type <tree> declList
%type <tree> decl
%type <tree> varDecl
%type <tree> scopedVarDecl 
%type <tree> varDeclList
%type <tree> varDeclInit
%type <tree> varDeclId
%type <tree> funDecl
%type <tree> parms
%type <tree> parmList
%type <tree> parmTypeList
%type <tree> parmId
%type <tree> parmIdList
%type <tree> stmt
%type <tree> matched
%type <tree> iterRange
%type <tree> unmatched
%type <tree> expstmt
%type <tree> compoundstmt
%type <tree> localDecls
%type <tree> stmtList
%type <tree> returnstmt
%type <tree> breakstmt
%type <tree> exp
%type <tree> simpleExp
%type <tree> andExp
%type <tree> unaryRelExp
%type <tree> relExp
%type <tree> minmaxExp
%type <tree> sumExp
%type <tree> mulExp
%type <tree> unaryExp
%type <tree> factor
%type <tree> mutable 
%type <tree> immutable
%type <tree> call
%type <tree> args
%type <tree> argList
%type <tree> constant

%type <tinfo> assignop relop minmaxop sumop mulop unaryop

%type <tinfo> '{' '}' '*' '+' '-' '/' '[' ']' ';' '>' '<' '=' ':' ',' '(' ')' '%' '?'

/*DEFAULT VALUE: in treeUtils.cpp, newDeclNode (and the other two) functions, default to SIZE OF 1*/
/* newNode->size = 1; */

/*DEFAULT VALUE: */

%token   <tokenData> FIRSTOP
%token   <tokenData> LASTOP

%token   <tinfo>  ADDASS
%token   <tinfo>  AND
%token   <tinfo>  BOOL
%token   <tinfo>  BOOLCONST
%token   <tinfo>  BREAK
%token   <tinfo>  BY
%token   <tinfo>  CHAR
%token   <tinfo>  CHARCONST
%token   <tinfo>  CHSIGN
%token   <tinfo>  DEC
%token   <tinfo>  DIVASS
%token   <tinfo>  DO
%token   <tinfo>  ELSE
%token   <tinfo>  EQ
%token   <tinfo>  ERROR
%token   <tinfo>  FOR
%token   <tinfo>  GEQ
%token   <tinfo>  ID
%token   <tinfo>  IF
%token   <tinfo>  INC
%token   <tinfo>  INT
%token   <tinfo>  LEQ
%token   <tinfo>  MAX
%token   <tinfo>  MIN
%token   <tinfo>  MULASS
%token   <tinfo>  NEQ
%token   <tinfo>  NOT
%token   <tinfo>  NUMCONST
%token   <tinfo>  OR
%token   <tinfo>  OP
%token   <tinfo>  PRECOMPILER
%token   <tinfo>  RETURN
%token   <tinfo>  SIZEOF
%token   <tinfo>  STATIC
%token   <tinfo>  STRINGCONST
%token   <tinfo>  SUBASS
%token   <tinfo>  THEN
%token   <tinfo>  TO
%token   <tinfo>  WHILE

%token   <tokenData> LASTTERM
%%

program : precomList declList {syntaxTree=$2;}
   ;

precomList : precomList PRECOMPILER {$$ = NULL; printf("%s\n", yylval.tinfo->tokenstr);} /*{ $$ = NULL; printf("%s\n");}*/
   | PRECOMPILER {$$ = NULL; printf("%s\n", yylval.tinfo->tokenstr);/*printf("%s\n");*/}
   | /* EMPTY */ {$$ = NULL;}
   ;

declList : declList decl {$$ = addSibling($1, $2);}
   | decl {$$ = $1;}
   ;

decl : varDecl {$$ = $1;}
   | funDecl {$$ = $1;}
   ;

/* Originally the only one w/ $$ = $2; setType($2, $1, false); yyerrok;*/
varDecl : typeSpec varDeclList ';' {$$ = $2; setType($2, $1, false); yyerrok;} 
   ;

scopedVarDecl : STATIC typeSpec varDeclList ';' {$$ = $3; setType($3, $2, true); yyerrok;}
   | typeSpec varDeclList ';' {$$ = $2; setType($2, $1, false); yyerrok;}
   ;

varDeclList : varDeclList ',' varDeclInit {$$ = addSibling($1, $3);}
   | varDeclInit {$$ = $1;}
   ;

varDeclInit : varDeclId {$$ = $1;}
   | varDeclId ':' simpleExp {$$ = $1; $$->child[0] = $3; setType($$, $3->type, false);} /*addtype(id.entry, $3->type);*/  
   ;

varDeclId : ID {$$ = newDeclNode(DeclKind::VarK, ExpType::UndefinedType, $1);}
   | ID '[' NUMCONST ']' {$$ = newDeclNode(DeclKind::VarK, ExpType::UndefinedType, $1); $$->isArray = true; $$->size = $3->nvalue + 1;} /*$$->type = array($1->type,$3)*/
   ;

typeSpec : INT {$$ = ExpType::Integer;} 
   | BOOL {$$ = ExpType::Boolean;}
   | CHAR {$$ = ExpType::Char;}
   ;

funDecl : typeSpec ID '(' parms ')' stmt {$$ = newDeclNode(DeclKind::FuncK, $1, $2, $4, $6); setType($6, $1, false);}
   |  ID '(' parms ')' stmt {$$ = newDeclNode(DeclKind::FuncK, ExpType::Void, $1, $3, $5);}
   ;

parms : parmList {$$ = $1;}
   | /* empty */{$$ = NULL;}
   ;

parmList : parmList ';' parmTypeList {$$ = addSibling($1, $3);}
   | parmTypeList {$$ = $1;}
   ;

parmTypeList : typeSpec parmIdList {$$ = $2; setType($2, $1, false); yyerrok;}
   ;

/* use addSibling when making a list */
parmIdList : parmIdList ',' parmId {$$ = addSibling($1, $3);}
   | parmId {$$ = $1;}
   ;

parmId : ID {$$ = newDeclNode(DeclKind::ParamK, ExpType::UndefinedType, $1);}
   | ID '['']' {$$ = newDeclNode(DeclKind::ParamK, ExpType::UndefinedType, $1); $$->isArray = true;}
   ;

stmt : matched {$$ = $1;}
   | unmatched {$$ = $1;}
   ;

matched : IF simpleExp THEN matched ELSE matched {$$ = newStmtNode(IfK, $1, $2, $4, $6);}
   | WHILE simpleExp DO matched {$$ = newStmtNode(WhileK, $1, $2, $4);}
   | FOR ID '=' iterRange DO matched {$$ = newStmtNode(StmtKind::ForK, $1, newDeclNode(DeclKind::VarK, ExpType::Integer, $2), $4, $6);}
   | expstmt {$$ = $1;}
   | compoundstmt {$$ = $1;}
   | returnstmt {$$ = $1;}
   | breakstmt {$$ = $1;} 
   ;

iterRange : simpleExp TO simpleExp {$$ = newStmtNode(StmtKind::RangeK, $2, $1, $3);}
   | simpleExp TO simpleExp BY simpleExp {$$ = newStmtNode(StmtKind::RangeK, $2, $1, $3, $5);}
   ;

unmatched : IF simpleExp THEN stmt {$$ = newStmtNode(IfK, $1, $2, $4);}
   | IF simpleExp THEN matched ELSE unmatched {$$ = newStmtNode(IfK, $1, $2, $4, $6);}
   | WHILE simpleExp DO unmatched {$$ = newStmtNode(WhileK, $1, $2, $4);}
   | FOR ID '=' iterRange DO unmatched  {$$ = newStmtNode(StmtKind::ForK, $1, newDeclNode(DeclKind::VarK, ExpType::Integer, $2), $4, $6);}
   ;

expstmt : exp ';' {$$ = $1;}
   | ';' {$$ = NULL;}
   ;

/*new scope*/
compoundstmt : '{' localDecls stmtList '}' {$$ = newStmtNode(CompoundK, $1, $2, $3); yyerrok;}
   ;

localDecls : localDecls scopedVarDecl {$$ = addSibling($1, $2);}
   |  /* empty */ {$$ = NULL;}
   ;

stmtList : stmtList stmt {$$ = ($2 == NULL ? $1 : addSibling($1, $2));}
   |  /* empty */ {$$ = NULL;}
   ;

returnstmt : RETURN ';' {$$ = newStmtNode(StmtKind::ReturnK, $1);}
   | RETURN exp ';' {$$ = newStmtNode(StmtKind::ReturnK, $1, $2);}
   ;

breakstmt : BREAK ';' {$$ = newStmtNode(StmtKind::BreakK, $1);}
   ;

exp : mutable assignop exp {$$ = newExpNode(ExpKind::AssignK, $2, $1, $3);}
   | mutable INC {$$ = newExpNode(ExpKind::AssignK, $2, $1);}
   | mutable DEC {$$ = newExpNode(ExpKind::AssignK, $2, $1);}
   | simpleExp {$$ = $1;}
   | mutable assignop error {$$ = NULL;}
   ;

assignop : '=' {$$ = $1;}
   | ADDASS {$$ = $1;}
   | SUBASS {$$ = $1;}
   | MULASS {$$ = $1;}
   | DIVASS {$$ = $1;}
   ;

simpleExp : simpleExp OR andExp {$$ = newExpNode(ExpKind::OpK, $2, $1, $3);}
   | andExp {$$ = $1;}
   ;

andExp : andExp AND unaryRelExp {$$ = newExpNode(ExpKind::OpK, $2, $1, $3);}
   | unaryRelExp {$$ = $1;}
   ;

unaryRelExp : NOT unaryRelExp {$$ = newExpNode(ExpKind::OpK, $1, $2);}
   | relExp {$$ = $1;}
   ;

relExp : minmaxExp relop minmaxExp {$$ = newExpNode(ExpKind::OpK, $2, $1, $3);}
   | minmaxExp {$$ = $1;}
   ;

relop : LEQ {$$ = $1;}
   | '<' {$$ = $1;}
   | '>' {$$ = $1;}
   | GEQ {$$ = $1;}
   | EQ {$$ = $1;}
   | NEQ {$$ = $1;}
   ;

minmaxExp : minmaxExp minmaxop sumExp {$$ = newExpNode(ExpKind::OpK, $2, $1, $3);}
   | sumExp {$$ = $1;}
   ;

minmaxop : MAX {$$ = $1;}
   | MIN {$$ = $1;}
   ;

sumExp : sumExp sumop mulExp {$$ = newExpNode(ExpKind::OpK, $2, $1, $3);}
   | mulExp {$$ = $1;}
   ;

sumop : '+' {$$ = $1;}
   | '-' {$$ = $1;}
   ;

mulExp : mulExp mulop unaryExp {$$ = newExpNode(ExpKind::OpK, $2, $1, $3);}
   | unaryExp {$$ = $1;}
   ;

mulop : '*' {$$ = $1;}
   | '/' {$$ = $1;}
   | '%' {$$ = $1;}
   ;

unaryExp : unaryop unaryExp {$$ = newExpNode(ExpKind::OpK, $1, $2);}
   | factor {$$ = $1;}
   ;

unaryop : '-' {$$ = $1; $$->tokenclass = CHSIGN;} /* chsign */
   | '*' {$$ = $1; $$->tokenclass = SIZEOF;} /* sizeof */
   | '?' {$$ = $1;}
   ;

factor : immutable {$$ = $1;}
   | mutable {$$ = $1;}
   ;

mutable : ID {$$ = newExpNode(ExpKind::IdK, $1); $$->attr.name = $1->svalue;}
   | ID '[' exp ']' {$$ = newExpNode(ExpKind::OpK, $2, newExpNode(ExpKind::IdK, $1), $3); $$->child[0]->attr.name = $1->svalue; $$->size = $3->size;} /*$$->type = array($1->type, $3);*/
   ;

immutable : '(' exp ')' {$$ = $2;}
   | call {$$ = $1;}
   | constant {$$ = $1;}
   ;

call : ID '(' args ')' {$$ = newExpNode(ExpKind::CallK, $1, $3);}
   ;

args : argList {$$ = $1;}
   | /* empty */ {$$ = NULL;}
   ;

argList : argList ',' exp {$$ = addSibling($1, $3);}
   | exp {$$ = $1;}
   ;

constant : NUMCONST {$$ = newExpNode(ExpKind::ConstantK, $1); $$->type = ExpType::Integer; $$->attr.value = $1->nvalue;} /*treeNode value -> tokenData value*/
   | CHARCONST {$$ = newExpNode(ExpKind::ConstantK, $1); $$->type = ExpType::Char; $$->attr.cvalue = $1->cvalue; $$->size = 1; $$->attr.string = $1->svalue;}
   | STRINGCONST {$$ = newExpNode(ExpKind::ConstantK, $1); $$->type = ExpType::Char; $$->attr.string = $1->svalue; $$->isArray = true; $$->size = $1->nvalue + 1;}
   | BOOLCONST {$$ = newExpNode(ExpKind::ConstantK, $1); $$->type = ExpType::Boolean; $$->size = 1; $$->attr.value = $1->nvalue;}
   ;

%%

void yyerror (const char *msg)
{ 
   cout << "Error: " <<  msg << endl;
}

char *largerTokens[LASTTERM+1];

void initTokenStrings()
{
   largerTokens[OP] = (char *)"op";
   largerTokens[EQ] = (char *)"==";
   largerTokens[NEQ] = (char *)"!=";
   largerTokens[LEQ] = (char *)"<=";
   largerTokens[GEQ] = (char *)">=";
   largerTokens[MULASS] = (char *)"*=";
   largerTokens[ADDASS] = (char *)"+=";
   largerTokens[SUBASS] = (char *)"-=";
   largerTokens[DIVASS] = (char *)"/=";
   largerTokens[INC] = (char *)"++";
   largerTokens[DEC] = (char *)"--";
   largerTokens[MAX] = (char *)":>:";
   largerTokens[MIN] = (char *)":<:";
   largerTokens[AND] = (char *)"and";
   largerTokens[CHAR] = (char *)"char";
   largerTokens[BOOL] = (char *)"bool";
   largerTokens[BREAK] = (char *)"break";
   largerTokens[BY] = (char *)"by";
   largerTokens[DO] = (char *)"do";
   largerTokens[ELSE] = (char *)"else";
   largerTokens[FOR] = (char *)"for";
   largerTokens[IF] = (char *)"if";
   largerTokens[INT] = (char *)"int";
   largerTokens[NOT] = (char *)"not";
   largerTokens[OR] = (char *)"or";
   largerTokens[RETURN] = (char *)"return";
   largerTokens[STATIC] = (char *)"static";
   largerTokens[THEN] = (char *)"then";
   largerTokens[TO] = (char *)"to";
   largerTokens[WHILE] = (char *)"while";
   largerTokens[PRECOMPILER] = (char *)"precompiler";
   largerTokens[NUMCONST] = (char *)"numconst";
   largerTokens[STRINGCONST] = (char *)"stringconst";
   largerTokens[ERROR] = (char *)"error";
   largerTokens[ID] = (char *)"id";
}

char *tokenToStr(int type)
{
   switch(type)
   {
      case '=':
         return (char *)"=";
         break;
      case '-':
         return (char *)"-";
         break;
      case '*':
         return (char *)"*";
         break;
      case ADDASS:
         return (char *)"+=";
         break;
      case SUBASS:
         return (char *)"-=";
         break;
      case MULASS:
         return (char *)"*=";
         break;
      case DIVASS:
         return (char *)"/=";
         break;
      case OR:
         return (char *)"or";
         break;
      case GEQ:
         return (char *)">=";
         break;
      case LEQ:
         return (char *)"<=";
         break;
      case EQ:
         return (char *)"==";
         break;
      case INC:
         return (char *)"++";
         break;
      case DEC:
         return (char *)"--";
         break;
      case MIN:
         return (char *)":<:";
         break;
      case MAX:
         return (char *)":>:";
         break;
      case NEQ:
         return (char *)"!=";
         break;
      case NOT:
         return (char *)"not";
         break;
      case AND:
         return (char *)"and";
         break;
      case CHSIGN:
         return (char *)"chsign";
         break;
      case SIZEOF:
         return (char *)"sizeof";
         break;
      default:
         if(int(type) < 256)
         {
            char tmp[2];
            tmp[0] = int(type);
            tmp[1] = 0;
            return strdup(tmp);
         }
         return largerTokens[type];
   }
}

int main(int argc, char **argv) {
   yylval.tinfo = (TokenData*)malloc(sizeof(TokenData));
   yylval.tree = (TreeNode*)malloc(sizeof(TreeNode));

   yylval.tinfo->linenum = 1;

   int option, index;
   int globalOffset = 0;
   char *file = NULL;
   bool dotAST = false;
   extern FILE *yyin;

   //symtab = new SymbolTable();
   //symtab->debug(false);
   //syntaxTree = semanticAnalysis(syntaxTree, shareCompoundSpace, noDuplicateUndefs, symtab, globalOffset);

   while ((option = getopt (argc, argv, "d")) != -1)
   {
      switch (option)
      {
      case 'd':
         dotAST = true;
         break;
      case '?':
      default:
         ;
      }
   }

   if ( optind == argc ) yyparse();
   for (index = optind; index < argc; index++) 
   {
      yyin = fopen (argv[index], "r");
      yyparse();
      fclose (yyin);
   }

   //syntaxTree = semanticAnalysis(syntaxTree, true, true, symtab, globalOffset);
   //codegen(stdout, argv[1], syntaxTree, symtab, globalOffset, false);
   // SymbolTable *symtab;
   // symtab = new SymbolTable();
   // symtab->debug(debugSymTab);

   if (numErrors == 0)
   {
      SymbolTable *symtab;
      symtab = new SymbolTable();
      symtab->debug(false);
      syntaxTree = semanticAnalysis(syntaxTree, true, false, symtab, globalOffset);
      codegen(stdout, argv[1], syntaxTree, symtab, globalOffset, false);

      //printTree(stdout, syntaxTree, true, true);
   }
   if (numErrors == 0) // &&
   {
      //Print semantic tree
   }
   else
   {
      printf("-----------\n");
      printf("Errors: %d\n", numErrors);
      printf("-----------\n");
   }

   printf("Number of warnings: %d\n", numWarnings);
   printf("Number of errors: %d\n", numErrors);

   return 0;
}
