#ifndef PARSERDEF
#define PARSERDEF

#define NUM_TERMINALS 61
#define NUM_NON_TERMINALS 53
#define MAX_PRODUCTIONS 200
#include "lexerDef.h"


typedef struct ParseTreeNode {
    char *symbol;                   // nonterminal or terminal name
    tokenInfo token;                // valid if the node is a terminal and has been matched
    struct ParseTreeNode **children; // array of pointers to children nodes
    int childCount;                 // number of children
} ParseTreeNode;

typedef struct {
    char *lhs;
    char **rhs;
    int rhsCount;
} Production;


// Global FOLLOW structure: each row corresponds to a nonterminal and each column to a terminal.
typedef struct {
    int table[NUM_NON_TERMINALS][NUM_TERMINALS];
} FollowSets;

// Global FIRST structure: each row corresponds to a nonterminal and each column to a terminal.
typedef struct {
    int table[NUM_NON_TERMINALS][NUM_TERMINALS];
} FirstSets;


#endif // !PARSERDEF