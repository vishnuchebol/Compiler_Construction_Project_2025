#ifndef LEXER
#define LEXER
#include "lexerDef.h"
#include <stdio.h>
FILE* getStream(FILE *fp,twinBuffer* TB);

void retract(int amt,twinBuffer* B);

/* Group 13
Virendrasinh Mane                           2022A7PS1175P
Vishnu Chebolu                              2022A7PS0124P
Shashwat Goyal                              2022A7PS0115P
Parthib Sarkar                              2022A7PS0111P
Samrath Singh Khanuja                       2022A7PS1171P
Harsimar Singh Saluja                       2022A7PS1187P*/
int checkSingleChar(char ch, char chToEqual);

void accept(twinBuffer* B);

void initBuffer(int size);

int getNextChar(twinBuffer* B);

void initLexer(FILE* f);



char* copyString(int start, int end);

tokenInfo createToken(TokenName tokenName,char* lexeme,int lineNumber,int isNumber,Value* value);

int checkInRange(char ch,char start, char end);
tokenInfo getNextToken();
void removeComments(char *testcaseFile, char *cleanFile);

#endif


