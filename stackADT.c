#include "stackADT.h"
#include <stdlib.h>

/* Group 13
Virendrasinh Mane                           2022A7PS1175P
Vishnu Chebolu                              2022A7PS0124P
Shashwat Goyal                              2022A7PS0115P
Parthib Sarkar                              2022A7PS0111P
Samrath Singh Khanuja                       2022A7PS1171P
Harsimar Singh Saluja                       2022A7PS1187P*/
Stack* createStack(int capacity) {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->nodes = (ParseTreeNode**)malloc(sizeof(ParseTreeNode*) * capacity);
    s->top = -1;
    s->capacity = capacity;
    return s;
}

void push(Stack* s, ParseTreeNode* node) {
    if (s->top == s->capacity - 1) {
        s->capacity *= 2;
        s->nodes = (ParseTreeNode**)realloc(s->nodes, sizeof(ParseTreeNode*) * s->capacity);
    }
    s->nodes[++(s->top)] = node;
}

ParseTreeNode* pop(Stack* s) {
    if(s->top < 0)
        return NULL;
    return s->nodes[(s->top)--];
}

ParseTreeNode* peek(Stack* s) {
    if(s->top < 0)
        return NULL;
    return s->nodes[s->top];
}
