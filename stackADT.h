#ifndef STACKADT
#define STACKADT
#include "stackADTDef.h"
#include "parserDef.h"
#include "lexerDef.h"

/* Group 13
Virendrasinh Mane                           2022A7PS1175P
Vishnu Chebolu                              2022A7PS0124P
Shashwat Goyal                              2022A7PS0115P
Parthib Sarkar                              2022A7PS0111P
Samrath Singh Khanuja                       2022A7PS1171P
Harsimar Singh Saluja                       2022A7PS1187P*/
Stack* createStack(int capacity);
void push(Stack* s, ParseTreeNode* node);
ParseTreeNode* pop(Stack* s);
ParseTreeNode* peek(Stack* s);

#endif // !STACKADT