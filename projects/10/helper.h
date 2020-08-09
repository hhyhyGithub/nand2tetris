#ifndef _HELPER_H
#define _HELPER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "dbg.h"

#define MAXTOKEN    100
#define MAXFILE     256
#define KEYWORD_NUM 21
#define SYMBOL_NUM  19
#define OP_NUM      9

/* Gloabal Variables */

extern int putback;
extern int is_LL1;
extern char token[MAXTOKEN];
extern int tokenType;

/* Utilities */

int is_directory(const char* filename);
char* setFileName(const char* arg);

/* JackTokenizer Module */

#define putBack() ((putback=1))
int has_more_token(FILE* fp);
void advance(FILE* fp);
int keyWord();
void tokenTest(FILE* fp, FILE* fw);

/* Compilation Module */

// compiling terminal
void compileKeyword(FILE* fp, FILE* fw, int options, char* info);
void compileSymbol(FILE* fp, FILE* fw, char* info);
void compileIntegerConstant(FILE* fp, FILE* fw);
void compileStringConstant(FILE* fp, FILE* fw);
void compileIdentifier(FILE* fp, FILE* fw, char* info);

// compiling nonterminal
void compileClass(FILE* fp, FILE* fw);
void compileClassVarDec(FILE* fp, FILE* fw);
void compileSubroutine(FILE* fp, FILE* fw);
void compileParameterList(FILE* fp, FILE* fw);
void compileVarDec(FILE* fp, FILE* fw);
void compileStatements(FILE* fp, FILE* fw);
void compileDo(FILE* fp, FILE* fw);
void compileLet(FILE* fp, FILE* fw);
void compileWhile(FILE* fp, FILE* fw);
void compileReturn(FILE* fp, FILE* fw);
void compileIf(FILE* fp, FILE* fw);
void compileExpression(FILE* fp, FILE* fw);
void compileTerm(FILE* fp, FILE* fw);
void compileExpressionList(FILE* fp, FILE* fw);
void compileSubroutineCall(FILE* fp, FILE* fw);

#endif