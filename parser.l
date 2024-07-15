%{
#include "scanType.h"
#include "treeNodes.h"
#include "parser.tab.h" // This has to be the last include
#include <stdlib.h>

using namespace std;

#define YY_DECL extern "C" int yylex()
#define YYSTYPE int

int line=1;
char *lastToken=(char *)"";
//extern int numErrors;
int atoi (const char * str);

int setValue(int linenum, int tokenClass, char *svalue) {
    yylval.tinfo = new TokenData;

    yylval.tinfo->tokenclass = tokenClass;
    yylval.tinfo->linenum = linenum;
    yylval.tinfo->tokenstr = strdup(svalue);
    yylval.tinfo->cvalue = svalue[0];
    yylval.tinfo->nvalue = atoi(svalue);
    yylval.tinfo->svalue = strdup(svalue);

    yylval.tinfo->tokenstr = lastToken = strdup(svalue);

    switch (tokenClass) {

    case NUMCONST:
         yylval.tinfo->nvalue = atoi(svalue);
         break;
      
    case BOOLCONST:  // Returns a 0 or 1 for false and true respectively
         yylval.tinfo->nvalue = ((lastToken[0]=='t') ? 1 : 0); 
         break;

    case CHARCONST:
         yylval.tinfo->cvalue = svalue[1];
         break;

    case STRINGCONST:
         //yylval.tinfo->svalue = svalue;
         yylval.tinfo->svalue = strdup(&svalue[1]);
         yylval.tinfo->nvalue = strlen(svalue)-2;
         yylval.tinfo->svalue[yylval.tinfo->nvalue] = '\0';
         //yylval.tinfo->svalue = strdup(&svalue[1]);
         //yylval.tinfo->svalue[yylva.]
         break;
    }

    return tokenClass;
}

%}

%option noyywrap

letter      [a-zA-Z]
digit    [0-9]
quote    [\']
underbar [_]
letdig      {digit}|{letter}|{underbar}
limitters   [\;\,\(\)\{\}\[\]]
op    [\-\+\*\/\%=]
whitespace  [\ \t]
%%
[\(\)\*\+\,\-\/\:\;\<\=\>\?\[\]\%\{\}] { return setValue(line, yytext[0], yytext); } // single char tokens

\'\\?.\'       { return setValue(line, CHARCONST, yytext); }

"//".*         ;

\"[^\"(\n)]*\" { return setValue(line, STRINGCONST, yytext); }

"--"           { return setValue(line, DEC, yytext); }

":>:"          { return setValue(line, MAX, yytext); }

":<:"          { return setValue(line, MIN, yytext); }

"!="           { return setValue(line, NEQ, yytext); }

"<="           { return setValue(line, LEQ, yytext); }

">="           { return setValue(line, GEQ, yytext); }

"*="           { return setValue(line, MULASS, yytext); }

"+="           { return setValue(line, ADDASS, yytext); }

"-="           { return setValue(line, SUBASS, yytext); }

"/="           { return setValue(line, DIVASS, yytext); }

"=="           { return setValue(line, EQ, yytext); }

"++"           { return setValue(line, INC, yytext); }

"char"         { return setValue(line, CHAR, yytext); }

"and"          { return setValue(line, AND, yytext); }

"bool"         { return setValue(line, BOOL, yytext); }

"break"        { return setValue(line, BREAK, yytext); }

"by"           { return setValue(line, BY, yytext); }

"do"           { return setValue(line, DO, yytext); }

"else"         { return setValue(line, ELSE, yytext); }

"false"        { return setValue(line, BOOLCONST, yytext); }

"true"         { return setValue(line, BOOLCONST, yytext); }

"for"          { return setValue(line, FOR, yytext); }

"if"           { return setValue(line, IF, yytext); }

"int"          { return setValue(line, INT, yytext); }

"not"          { return setValue(line, NOT, yytext); }

"or"           { return setValue(line, OR, yytext); }

"return"       { return setValue(line, RETURN, yytext); }

"static"       { return setValue(line, STATIC, yytext); }

"then"         { return setValue(line, THEN, yytext); }

"to"           { return setValue(line, TO, yytext); }

"while"        { return setValue(line, WHILE, yytext); }

[a-zA-Z][a-zA-Z0-9]* { return setValue(line, ID, yytext); }

^("#DRBC "|"#DRBCRUN ").*    { return setValue(line, PRECOMPILER, yytext); }

[0-9]+         { return setValue(line, NUMCONST, yytext); }
\n             {  line++; }
{whitespace}+  ;
.              { printf("ERROR(%d): Token error, invalid or misplaced input character: \'%c\'. Character Ignored.\n", line, yytext[0]);}


