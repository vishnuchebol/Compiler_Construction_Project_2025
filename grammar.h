#ifndef GRAMMAR
#define GRAMMAR

// Enum to distinguish between terminals and non-terminals
typedef enum {
    TERMINAL,
    NONTERMINAL
} SymbolType;

// Structure to represent a symbol in the grammar (either terminal or non-terminal)
typedef struct {
    SymbolType type;
    char *name; // Name of the symbol (e.g., "E", "id", "+")
} Symbol;

// Structure to represent a production rule in the grammar
// For example: A -> alpha, where alpha is a sequence of symbols
typedef struct {
    char *lhs;        // Left-hand side non-terminal (e.g., "A")
    Symbol **rhs;     // Array of pointers to symbols on the right-hand side
    int rhsCount;     // Number of symbols in the RHS array
} Production;

// Structure to represent the entire grammar
typedef struct {
    char **nonTerminals;  // Array of names of non-terminals
    int nonTerminalCount; // Number of non-terminals
    char **terminals;     // Array of names of terminals
    int terminalCount;    // Number of terminals
    Production *productions;  // Array of productions
    int productionCount;      // Number of productions
    char *startSymbol;        // The start symbol of the grammar
} Grammar;

// A simple structure to represent a set of strings.
// In practice you might want a more robust set implementation.
typedef struct {
    char **elements;  // Array of string elements
    int count;        // Number of elements in the set
} StringSet;

// Structure to hold FIRST and FOLLOW sets for each non-terminal.
// The index in the first and follow arrays corresponds to a non-terminal in grammar->nonTerminals.
typedef struct {
    StringSet *first;  // Array of FIRST sets for each non-terminal
    StringSet *follow; // Array of FOLLOW sets for each non-terminal
    int count;         // Number of non-terminals (should match grammar->nonTerminalCount)
} FirstAndFollow;


#endif // !GRAMMAR
