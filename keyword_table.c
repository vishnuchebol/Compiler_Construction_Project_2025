#include "interface.h"
#include "keyword_table.h"
#include <string.h>
#define NUMBER_KEYWORDS 28

// int hashFunction(char* str) {

//     /*Hash function string sum and mod */
//     // int sum = 0;
//     // for(int i=0; i < strlen(str); i++) {
//     //     sum += str[i]-'0';
//     // }
//     // return (sum%NUMBER_KEYWORDS);

//     /* Hash function djb2 and mod */
//     unsigned long hash = 5381;
//     int c;
//     while (c = *str++)
//         hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//     return (hash%NUMBER_KEYWORDS);
// }

int hashFunction(char* str) {
    if (str == NULL) {
        return 0; // or handle error as needed
    }
    
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {  // Using extra parentheses for clarity
        hash = ((hash << 5) + hash) + c;  // Equivalent to hash * 33 + c
    }
    
    return (int)(hash % NUMBER_KEYWORDS);
}


void addEntry(KeywordTable* kt,TokenName tn, char* lexeme) {
    int hash =  hashFunction(lexeme);
    // printf("Hash is %d for the keyword %s\n" , hash, lexeme);
    kt->KEYWORDS[hash].keywords = addToList(kt->KEYWORDS[hash].keywords,tn,lexeme);
}

// Node* lookUp(KeywordTable* kt,char* lexeme) {
//     int hash = hashFunction(lexeme);
//     Node* trav = kt->KEYWORDS[hash].keywords;
//     while(trav != NULL) {
//         if(strcmp(lexeme,trav->LEXEME) == 0)
//             return trav;
//         trav = trav->next;
//     }

//     return NULL;
// }

Node* lookUp(KeywordTable* kt, char* lexeme) {
    //printf("lexeme is %s\n",lexeme);
    if (kt == NULL || lexeme == NULL)
        return NULL;
    
    int hash = hashFunction(lexeme);
    int index = hash % NUMBER_KEYWORDS; 
    if (index < 0)
        index += NUMBER_KEYWORDS;       

    Node* trav = kt->KEYWORDS[index].keywords;
    while(trav != NULL) {
        if(strcmp(lexeme, trav->LEXEME) == 0)
            return trav;
        trav = trav->next;
    }
    return NULL;
}


KeywordTable* initializeTable() {
    KeywordTable* kt = (KeywordTable*)malloc(NUMBER_KEYWORDS*sizeof(KeywordTable));
    kt->KEYWORDS = (KeywordNode*)malloc(NUMBER_KEYWORDS*sizeof(KeywordNode));

    for(int i=0; i < NUMBER_KEYWORDS; i++) {
        kt->KEYWORDS[i].keywords = NULL;
    }

    addEntry(kt,TK_WITH,"with");
    addEntry(kt,TK_PARAMETERS,"parameters");
    addEntry(kt,TK_END,"end");
    addEntry(kt,TK_WHILE,"while");
    addEntry(kt,TK_UNION,"union");
    addEntry(kt,TK_ENDUNION,"end union");
    addEntry(kt,TK_DEFINETYPE,"definetype");
    addEntry(kt,TK_AS,"as");
    addEntry(kt,TK_TYPE,"type");
    addEntry(kt,TK_MAIN,"_main");
    addEntry(kt,TK_GLOBAL,"global");
    addEntry(kt,TK_PARAMETER,"parameter");
    addEntry(kt,TK_LIST,"list");
    addEntry(kt,TK_INPUT,"input");
    addEntry(kt,TK_OUTPUT,"output");
    addEntry(kt,TK_INT,"int");
    addEntry(kt,TK_REAL,"real");
    addEntry(kt,TK_ENDWHILE,"endwhile");
    addEntry(kt,TK_IF,"if");
    addEntry(kt,TK_THEN,"then");
    addEntry(kt,TK_ENDIF,"endif");
    addEntry(kt,TK_READ,"read");
    addEntry(kt,TK_WRITE,"write");
    addEntry(kt,TK_RETURN,"return");
    addEntry(kt,TK_CALL,"call");
    addEntry(kt,TK_RECORD,"record");
    addEntry(kt,TK_ENDRECORD,"endrecord");
    addEntry(kt,TK_ELSE,"else");

    return kt;

}

Node* addToList(Node* ls, TokenName tn, char* lexeme) {

    if(ls == NULL) {
        Node* n  = (Node*)malloc(sizeof(Node));
        n->LEXEME = lexeme;
        n->TOKEN_NAME = tn;
        n->next = NULL;
        return n;
    }

    Node* n = (Node*)malloc(sizeof(Node));
    n->LEXEME = lexeme;
    n->TOKEN_NAME = tn;
    n->next = ls;
    return n;
}

int searchList(Node* ls, char* lexeme) {
    Node* trav = ls;
    while(trav != NULL) {
        if(strcmp(lexeme,trav->LEXEME))
            return 1;
        trav = trav->next;
    }

    return 0;
}

int printList(Node* ls) {
    Node* trav = ls;
    int len = 0;
    if(ls == NULL) {
        printf("This slot is not occupied!\n");
        printf("\n");
        return 0;
    }

    while(trav != NULL) {
        printf("Keyword: %s " ,trav->LEXEME);
        trav = trav->next;
        len++;
    }
    printf("\n");
    printf("\n");
    return len;
}

void printHashTable(KeywordTable* kt) {
    int empty = 0;
    int collisions = 0;
    for(int i=0; i < NUMBER_KEYWORDS; i++) {
        int len = printList(kt->KEYWORDS[i].keywords);
        if(len == 0)
            empty++;
        if(len > 1)
            collisions += len-1;
    }

    printf("\n");
    printf("Calculating laod-factor\n");
    printf("%f\n" ,((float)empty)/NUMBER_KEYWORDS);

    printf("\n");
    printf("Calculating total collisions\n");
    printf("%d\n" , collisions);


}