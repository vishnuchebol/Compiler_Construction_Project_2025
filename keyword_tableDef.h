#ifndef KEYWORD_TABLE_DEF_
#define KEYWORD_TABLE_DEF_

#include "lexerDef.h"

/* Group 13
Virendrasinh Mane                           2022A7PS1175P
Vishnu Chebolu                              2022A7PS0124P
Shashwat Goyal                              2022A7PS0115P
Parthib Sarkar                              2022A7PS0111P
Samrath Singh Khanuja                       2022A7PS1171P
Harsimar Singh Saluja                       2022A7PS1187P*/
// Node actually storing the keyword!
typedef struct Node {
    TokenName TOKEN_NAME;
    char* LEXEME;
    struct Node* next;
} Node;

// Each slot contains a linked list
// The linked list is empty if the slot does not have any key
typedef struct KeywordNode {
    Node* keywords;
} KeywordNode;

// An array of slots where keywords are hashed.
// In case of a collision the hashed slot adds the entry as a linked list
typedef struct KeywordTable {
    KeywordNode* KEYWORDS;
} KeywordTable;

#endif