#ifndef KEYWORD_TABLE
#define KEYWORD_TABLE
#include "keyword_tableDef.h"

// Functions for keyword hashtable

/* Group 13
Virendrasinh Mane                           2022A7PS1175P
Vishnu Chebolu                              2022A7PS0124P
Shashwat Goyal                              2022A7PS0115P
Parthib Sarkar                              2022A7PS0111P
Samrath Singh Khanuja                       2022A7PS1171P
Harsimar Singh Saluja                       2022A7PS1187P*/
int hashFunction(char* str);
void addEntry(KeywordTable* kt,TokenName tn, char* lexeme);
Node* lookUp(KeywordTable* kt,char* lexeme);
KeywordTable* initializeTable();
void printHashTable(KeywordTable* kt);
// Functions for List
Node* addToList(Node* ls, TokenName tn, char* lexeme);
int searchList(Node* ls, char* lexeme);
int printList(Node* ls);

#endif //KEYWORD