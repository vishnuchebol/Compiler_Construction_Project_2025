#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "parser.h"
#include "stackADT.h"
int donesd = 1;

Production productions[MAX_PRODUCTIONS];
FirstSets firstSets;
FollowSets followSets;
int prodCount = 0;

// Global array of terminals
const char *terminals[NUM_TERMINALS] = {
    "TK_UNKNOWN", "TK_ASSIGNOP", "TK_COMMENT", "TK_ID", "TK_RUID",
    "TK_UNION", "TK_ENDUNION", "TK_DEFINETYPE", "TK_AS", "TK_NUM",
    "TK_RNUM", "TK_FIELDID", "TK_FUNID", "TK_WITH", "TK_PARAMETERS",
    "TK_END", "TK_WHILE", "TK_TYPE", "TK_MAIN", "TK_GLOBAL", "TK_PARAMETER",
    "TK_LIST", "TK_SQL", "TK_SQR", "TK_INPUT", "TK_OUTPUT", "TK_INT",
    "TK_REAL", "TK_COMMA", "TK_SEM", "TK_COLON", "TK_DOT", "TK_ENDWHILE",
    "TK_OP", "TK_CL", "TK_IF", "TK_THEN", "TK_ENDIF", "TK_READ", "TK_WRITE",
    "TK_RETURN", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_CALL",
    "TK_RECORD", "TK_ENDRECORD", "TK_ELSE", "TK_AND", "TK_OR", "TK_NOT",
    "TK_LT", "TK_LE", "TK_EQ", "TK_GT", "TK_GE", "TK_NE", "TK_ERR",
    "TK_EPS", "TK_DOLLAR"
};

// Global array of nonterminals
char *nonterminals[NUM_NON_TERMINALS] = {
    "program",
    "otherFunctions",
    "mainFunction",
    "stmts",
    "function",
    "input_par",
    "output_par",
    "parameter_list",
    "dataType",
    "remaining_list",
    "primitiveDatatype",
    "constructedDatatype",
    "typeDefinitions",
    "declarations",
    "otherStmts",
    "returnStmt",
    "actualOrRedefined",
    "typeDefinition",
    "definetypestmt",
    "fieldDefinitions",
    "fieldDefinition",
    "moreFields",
    "fieldtype",
    "declaration",
    "global_or_not",
    "stmt",
    "assignmentStmt",
    "iterativeStmt",
    "conditionalStmt",
    "ioStmt",
    "funCallStmt",
    "singleOrRecId",
    "arithmeticExpression",
    "option_single_constructed",
    "oneExpansion",
    "moreExpansions",
    "outputParameters",
    "inputParameters",
    "idList",
    "booleanExpression",
    "elsePart",
    "var",
    "term",
    "expPrime",
    "factor",
    "termPrime",
    "highPrecedenceOperators",
    "lowPrecedenceOperators",
    "logicalOp",
    "relationalOp",
    "optionalReturn",
    "more_ids",
    "A"
};


// Helper: returns the index of a nonterminal in nonterminals[]
int getNonterminalIndex(const char* nt) {
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        if (strcmp(nonterminals[i], nt) == 0)
            return i;
    }
    return -1;
}

// Helper: returns the index of a terminal in terminals[]
int getTerminalIndex(const char* t) {
    for (int i = 0; i < NUM_TERMINALS; i++) {
        if (strcmp(terminals[i], t) == 0)
            return i;
    }
    return -1;
}

// Reads the grammar from a file and stores productions.
void readGrammar(char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }
    
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline and skip empty lines.
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0)
            continue;
        
        // Tokenize the line using strtok_r.
        char* tokens[100];
        int tokenCount = 0;
        char *saveptr;
        char *tok = strtok_r(line, " \t", &saveptr);
        while (tok && tokenCount < 100) {
            tokens[tokenCount++] = strdup(tok);
            tok = strtok_r(NULL, " \t", &saveptr);
        }
        
        // Must have at least one LHS and one RHS symbol.
        if (tokenCount < 2) {
            for (int i = 0; i < tokenCount; i++)
                free(tokens[i]);
            continue;
        }
        
        if (prodCount >= MAX_PRODUCTIONS) {
            fprintf(stderr, "Exceeded maximum productions.\n");
            break;
        }
        
        // Create a production: first token is LHS; remaining tokens form the RHS.
        productions[prodCount].lhs = tokens[0]; // Keep tokens[0] as LHS.
        productions[prodCount].rhsCount = tokenCount - 1;
        productions[prodCount].rhs = malloc(sizeof(char*) * (tokenCount - 1));
        for (int i = 1; i < tokenCount; i++) {
            productions[prodCount].rhs[i-1] = tokens[i];
        }
        prodCount++;
    }
    fclose(fp);
}

// Computes the FIRST sets with full propagation (handling nonterminals on the RHS).
void computeFirstSets() {
    // Initialize FIRST table to 0.
    for (int i = 0; i < NUM_NON_TERMINALS; i++)
        for (int j = 0; j < NUM_TERMINALS; j++)
            firstSets.table[i][j] = 0;
    
    int changed = 1;
    while (changed) {
        changed = 0;
        // Process each production A -> X1 X2 ... Xn
        for (int p = 0; p < prodCount; p++) {
            int lhsIndex = getNonterminalIndex(productions[p].lhs);
            if (lhsIndex == -1)
                continue;
            int canDeriveEps = 1;
            for (int i = 0; i < productions[p].rhsCount && canDeriveEps; i++) {
                char *sym = productions[p].rhs[i];
                // If symbol is a terminal
                if (strncmp(sym, "TK_", 3) == 0) {
                    int tIndex = getTerminalIndex(sym);
                    if (tIndex != -1 && !firstSets.table[lhsIndex][tIndex]) {
                        firstSets.table[lhsIndex][tIndex] = 1;
                        changed = 1;
                    }
                    // If the terminal is TK_EPS, we continue; otherwise, break.
                    if (strcmp(sym, "TK_EPS") == 0)
                        canDeriveEps = 1;
                    else
                        canDeriveEps = 0;
                } else {
                    // Symbol is a nonterminal.
                    int ntIndex = getNonterminalIndex(sym);
                    if (ntIndex == -1)
                        continue;
                    // Add FIRST(nonterminal) except TK_EPS to FIRST(lhs).
                    for (int t = 0; t < NUM_TERMINALS; t++) {
                        // Skip TK_EPS (if present, will be handled later)
                        if (strcmp(terminals[t], "TK_EPS") == 0)
                            continue;
                        if (firstSets.table[ntIndex][t] && !firstSets.table[lhsIndex][t]) {
                            firstSets.table[lhsIndex][t] = 1;
                            changed = 1;
                        }
                    }
                    // If FIRST(nonterminal) contains TK_EPS then continue; otherwise stop.
                    int epsIndex = getTerminalIndex("TK_EPS");
                    if (epsIndex != -1 && firstSets.table[ntIndex][epsIndex])
                        canDeriveEps = 1;
                    else
                        canDeriveEps = 0;
                }
            }
            // If all symbols in RHS can derive ε, then add TK_EPS to FIRST(lhs).
            if (canDeriveEps) {
                int epsIndex = getTerminalIndex("TK_EPS");
                if (epsIndex != -1 && !firstSets.table[lhsIndex][epsIndex]) {
                    firstSets.table[lhsIndex][epsIndex] = 1;
                    changed = 1;
                }
            }
        }
    }
}

// Prints the FIRST set for each nonterminal.
void printFirstSets() {
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        printf("FIRST(%s) = { ", nonterminals[i]);
        int printed = 0;
        for (int j = 0; j < NUM_TERMINALS; j++) {
            if (firstSets.table[i][j]) {
                printf("%s ", terminals[j]);
                printed = 1;
            }
        }
        if (!printed)
            printf("∅");
        printf("}\n");
    }
}

// Computes the FOLLOW sets for all nonterminals using the productions and the FIRST sets.
void computeFollowSets() {
    // Initialize the follow table to 0.
    for (int i = 0; i < NUM_NON_TERMINALS; i++)
        for (int j = 0; j < NUM_TERMINALS; j++)
            followSets.table[i][j] = 0;
    
    // For the start symbol (assumed to be nonterminals[0]), add the end-marker (TK_DOLLAR).
    int startIndex = getNonterminalIndex(nonterminals[0]);
    int dollarIndex = getTerminalIndex("TK_DOLLAR");
    if (startIndex != -1 && dollarIndex != -1) {
        followSets.table[startIndex][dollarIndex] = 1;
    }
    
    int changed = 1;
    while (changed) {
        changed = 0;
        // For each production A -> X1 X2 ... Xn
        for (int p = 0; p < prodCount; p++) {
            int A_index = getNonterminalIndex(productions[p].lhs);
            // Process each symbol in the RHS.
            for (int i = 0; i < productions[p].rhsCount; i++) {
                char *sym = productions[p].rhs[i];
                // Only process if sym is a nonterminal (i.e. does not start with "TK_").
                if (strncmp(sym, "TK_", 3) == 0)
                    continue;
                int B_index = getNonterminalIndex(sym);
                if (B_index == -1)
                    continue;
                // Compute FIRST(beta) where beta = X(i+1)...Xn.
                int canDeriveEps = 1;
                // For each symbol in beta:
                for (int j = i + 1; j < productions[p].rhsCount && canDeriveEps; j++) {
                    char *beta_sym = productions[p].rhs[j];
                    // If beta_sym is terminal:
                    if (strncmp(beta_sym, "TK_", 3) == 0) {
                        int tIndex = getTerminalIndex(beta_sym);
                        if (tIndex != -1) {
                            // Add beta_sym to FOLLOW(B) if not TK_EPS.
                            if (strcmp(beta_sym, "TK_EPS") != 0 && !followSets.table[B_index][tIndex]) {
                                followSets.table[B_index][tIndex] = 1;
                                changed = 1;
                            }
                        }
                        // Only continue if beta_sym is TK_EPS.
                        if (strcmp(beta_sym, "TK_EPS") == 0)
                            canDeriveEps = 1;
                        else
                            canDeriveEps = 0;
                    }
                    else { // beta_sym is a nonterminal.
                        int beta_nt = getNonterminalIndex(beta_sym);
                        if (beta_nt == -1)
                            continue;
                        // For each terminal in FIRST(beta_sym) except TK_EPS, add it to FOLLOW(B).
                        for (int t = 0; t < NUM_TERMINALS; t++) {
                            // Skip TK_EPS.
                            if (strcmp(terminals[t], "TK_EPS") == 0)
                                continue;
                            if (firstSets.table[beta_nt][t] && !followSets.table[B_index][t]) {
                                followSets.table[B_index][t] = 1;
                                changed = 1;
                            }
                        }
                        // Continue if FIRST(beta_sym) contains TK_EPS.
                        int epsIndex = getTerminalIndex("TK_EPS");
                        if (epsIndex != -1 && firstSets.table[beta_nt][epsIndex])
                            canDeriveEps = 1;
                        else
                            canDeriveEps = 0;
                    }
                }
                // If beta is empty or all symbols in beta can derive ε,
                // then add FOLLOW(A) to FOLLOW(B).
                if (canDeriveEps) {
                    for (int t = 0; t < NUM_TERMINALS; t++) {
                        if (followSets.table[A_index][t] && !followSets.table[B_index][t]) {
                            followSets.table[B_index][t] = 1;
                            changed = 1;
                        }
                    }
                }
            }
        }
    }
}

// Prints the FOLLOW set for each nonterminal.
void printFollowSets() {
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        printf("FOLLOW(%s) = { ", nonterminals[i]);
        int printed = 0;
        for (int j = 0; j < NUM_TERMINALS; j++) {
            if (followSets.table[i][j]) {
                printf("%s ", terminals[j]);
                printed = 1;
            }
        }
        if (!printed)
            printf("∅");
        printf("}\n");
    }
}

// --- PARSE TABLE STRUCTURE AND FUNCTIONS ---

// Global parse table: rows correspond to nonterminals and columns to terminals.
// Each cell holds the index of the production to use, or -1 if none.
int parseTable[NUM_NON_TERMINALS][NUM_TERMINALS];

// Computes the parse table using the productions along with FIRST and FOLLOW sets.
void computeParseTable() {
    // Initialize parse table cells to -1 (meaning no production).
    for (int i = 0; i < NUM_NON_TERMINALS; i++)
        for (int j = 0; j < NUM_TERMINALS; j++)
            parseTable[i][j] = -1;
    
    // For each production A -> α, determine FIRST(α) and then fill the table.
    for (int p = 0; p < prodCount; p++) {
        int A_index = getNonterminalIndex(productions[p].lhs);
        if (A_index == -1)
            continue;
        
        // Compute FIRST(α) for production p.
        // We'll compute it into an array firstAlpha where firstAlpha[t]==1 means terminals[t] is in FIRST(α)
        int firstAlpha[NUM_TERMINALS] = {0};
        int canDeriveEps = 1; // true if all symbols seen so far can derive ε.
        for (int i = 0; i < productions[p].rhsCount && canDeriveEps; i++) {
            char *sym = productions[p].rhs[i];
            if (strncmp(sym, "TK_", 3) == 0) { // sym is terminal
                int tIndex = getTerminalIndex(sym);
                if (tIndex != -1)
                    firstAlpha[tIndex] = 1;
                // Only continue if the terminal is TK_EPS.
                if (strcmp(sym, "TK_EPS") == 0)
                    canDeriveEps = 1;
                else
                    canDeriveEps = 0;
            } else {
                // sym is a nonterminal; add FIRST(sym) except TK_EPS.
                int ntIndex = getNonterminalIndex(sym);
                if (ntIndex != -1) {
                    for (int t = 0; t < NUM_TERMINALS; t++) {
                        // Skip TK_EPS (if present)
                        if (strcmp(terminals[t], "TK_EPS") == 0)
                            continue;
                        if (firstSets.table[ntIndex][t])
                            firstAlpha[t] = 1;
                    }
                    // Continue if FIRST(sym) contains TK_EPS.
                    int epsIndex = getTerminalIndex("TK_EPS");
                    if (epsIndex != -1 && firstSets.table[ntIndex][epsIndex])
                        canDeriveEps = 1;
                    else
                        canDeriveEps = 0;
                }
            }
        }
        
        // For every terminal a in FIRST(α) (except TK_EPS), add production p to table[A_index][a].
        for (int t = 0; t < NUM_TERMINALS; t++) {
            if (firstAlpha[t]) {
                if (parseTable[A_index][t] != -1) {
                    printf("Conflict in parse table for nonterminal %s, terminal %s\n", nonterminals[A_index], terminals[t]);
                }
                parseTable[A_index][t] = p;
            }
        }
        
        // If α can derive ε (i.e. canDeriveEps is true), then for each terminal b in FOLLOW(A), add production p.
        if (canDeriveEps) {
            for (int t = 0; t < NUM_TERMINALS; t++) {
                if (followSets.table[A_index][t]) {
                    if (parseTable[A_index][t] != -1) {
                        printf("Conflict in parse table for nonterminal %s, terminal %s\n", nonterminals[A_index], terminals[t]);
                    }
                    parseTable[A_index][t] = p;
                }
            }
        }
    }
}

// Prints the parse table.
void printParseTable() {
    printf("Parse Table:\n");
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        printf("%s: ", nonterminals[i]);
        for (int j = 0; j < NUM_TERMINALS; j++) {
            if (parseTable[i][j] != -1) {
                // Print the terminal and the production index.
                printf("[%s: p%d] ", terminals[j], parseTable[i][j]);
            }
        }
        printf("\n");
    }
}

//---------------------//
// Parse Tree Structures
//---------------------//


ParseTreeNode* createNode(const char* symbol) {
    ParseTreeNode* node = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
    node->symbol = strdup(symbol);
    node->childCount = 0;
    node->children = NULL;
    return node;
}

//---------------------//
// ParseInputSourceCode Function
//---------------------//
// This function builds a parse tree using the global parseTable and productions,
// and uses getNextToken() to obtain tokens from the lexer.
// It now skips tokens of type TK_COMMENT.
ParseTreeNode* parseInputSourceCode() {
    // Create a stack with an initial capacity.
    Stack* stack = createStack(100);
    
    // Create the root parse tree node with the start symbol.
    ParseTreeNode* root = createNode(nonterminals[0]);
    push(stack, root);
    
    // Get the first token from the lexer.
    tokenInfo currToken = getNextToken();
    
    // Define a recovery limit constant.
    const int RECOVERY_LIMIT = 20;
    
    while (stack->top != -1) {
        // Skip comment tokens.
        while (currToken.TOKEN_NAME == TK_COMMENT) {
            currToken = getNextToken();
        }
        
        ParseTreeNode* topNode = peek(stack);
        
        // If the top node is a terminal.
        if (strncmp(topNode->symbol, "TK_", 3) == 0) {
            int expectedTermIndex = getTerminalIndex(topNode->symbol);
            if (expectedTermIndex == currToken.TOKEN_NAME) {
                // Terminal matches: attach token info and pop the node.
                topNode->token = currToken;
                pop(stack);
                currToken = getNextToken();
                continue;
            } else {
                // Terminal mismatch: print error, pop the node, and recover.
                printf("Line %d Error: Expected %s, got %s (lexeme: %s)\n", 
                    currToken.LINE_NO, topNode->symbol, 
                    terminals[currToken.TOKEN_NAME], currToken.LEXEME);
                donesd = 0;
                pop(stack);
                
                // Panic mode recovery: skip tokens until expected token or TK_DOLLAR is encountered.
                int recoveryCounter = 0;
                int errorLine = currToken.LINE_NO;
                while (currToken.TOKEN_NAME != expectedTermIndex &&
                       currToken.TOKEN_NAME != TK_DOLLAR &&
                       currToken.LINE_NO == errorLine &&
                       recoveryCounter < RECOVERY_LIMIT) {
                    currToken = getNextToken();
                    recoveryCounter++;
                }
                // If recovery limit is reached, skip the rest of the line.
                if (recoveryCounter >= RECOVERY_LIMIT) {
                    while (currToken.TOKEN_NAME != TK_DOLLAR && currToken.LINE_NO == errorLine) {
                        currToken = getNextToken();
                    }
                }
                continue;
            }
        } else {
            // The top node is a nonterminal.
            int ntIndex = getNonterminalIndex(topNode->symbol);
            int currTermIndex = currToken.TOKEN_NAME;
            
            // Consult the parse table to get the production index.
            int prodIndex = parseTable[ntIndex][currTermIndex];
            if (prodIndex == -1) {
                printf("Line %d Error: No production for nonterminal %s on terminal %s\n",
                       currToken.LINE_NO, topNode->symbol, terminals[currTermIndex]);
                donesd = 0;
                // Panic mode recovery:
                // If the current token is in FOLLOW(nt) then pop the nonterminal;
                // otherwise, skip tokens until one in FOLLOW(nt) or recovery limit reached.
                int recoveryCounter = 0;
                int errorLine = currToken.LINE_NO;
                while (currToken.TOKEN_NAME != TK_DOLLAR &&
                       followSets.table[ntIndex][currToken.TOKEN_NAME] == 0 &&
                       currToken.LINE_NO == errorLine &&
                       recoveryCounter < RECOVERY_LIMIT) {
                    currToken = getNextToken();
                    recoveryCounter++;
                }
                if (recoveryCounter >= RECOVERY_LIMIT) {
                    while (currToken.TOKEN_NAME != TK_DOLLAR && currToken.LINE_NO == errorLine) {
                        currToken = getNextToken();
                    }
                }
                // Once synchronized, pop the nonterminal.
                pop(stack);
                continue;
            }
            
            pop(stack);
            Production prod = productions[prodIndex];
            
            // Create children nodes for each symbol in the production's RHS.
            topNode->childCount = prod.rhsCount;
            topNode->children = (ParseTreeNode**)malloc(sizeof(ParseTreeNode*) * prod.rhsCount);
            for (int i = 0; i < prod.rhsCount; i++) {
                ParseTreeNode* child = createNode(prod.rhs[i]);
                topNode->children[i] = child;
            }
            
            // Push children in reverse order onto the stack.
            // For epsilon productions (i.e. symbol "TK_EPS"), do not push them.
            for (int i = prod.rhsCount - 1; i >= 0; i--) {
                if (strcmp(topNode->children[i]->symbol, "TK_EPS") == 0)
                    continue;
                push(stack, topNode->children[i]);
            }
        }
    }
    
    printf("Parsing done successfully\n");
    free(stack->nodes);
    free(stack);
    return root;
}


void initParser(char * filename){
	readGrammar(filename);
    computeFirstSets();
    computeFollowSets();
    computeParseTable();
    //printParseTable();
    //printFollowSets();
    //printFirstSets();
    ParseTreeNode* root = parseInputSourceCode();
    if(donesd == 0){
        return;
    }
	printParseTreeInOrder(root,0);
	
}

// Recursively prints the parse tree with indentation.
void printParseTree(ParseTreeNode* node, int indent) {
    if (node == NULL) return;
    
    // Print indentation.
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    
    // Print the node's symbol.
    printf("%s", node->symbol);
    
    // If this is a terminal node (we assume terminals start with "TK_"),
    // and if it has a token lexeme, print it.
    if (strncmp(node->symbol, "TK_", 3) == 0) {
        if (node->token.LEXEME != NULL) {
            printf(" (%s)", node->token.LEXEME);
        }
    }
    
    printf("\n");
    
    // Recursively print each child.
    for (int i = 0; i < node->childCount; i++) {
        printParseTree(node->children[i], indent + 1);
    }
}

// Recursively prints the parse tree with indentation.
void printParseTreeInOrder(ParseTreeNode* node, int indent) {
    if (node == NULL) return;
    
    // If there is at least one child, visit the leftmost child first.
    if (node->childCount > 0) {
        printParseTreeInOrder(node->children[0], indent + 1);
    }
    
    // Print the current node with indentation.
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    printf("%s", node->symbol);
    if (strncmp(node->symbol, "TK_", 3) == 0 && node->token.LEXEME != NULL) {
        printf(" (%s)", node->token.LEXEME);
    }
    printf("\n");
    
    // Then visit the remaining children (if any).
    for (int i = 1; i < node->childCount; i++) {
        printParseTreeInOrder(node->children[i], indent+1);
    }
}


// int main(int argc, char *argv[]) {
//     if(argc < 2) {
//         fprintf(stderr, "Usage: %s grammar.txt\n", argv[0]);
//         return 1;
//     }
    
//     readGrammar(argv[1]);
//     computeFirstSets();
// 	computeFollowSets();
// 	computeParseTable();
// 	//printParseTable();
// 	//printFollowSets();
//     //printFirstSets();
// 	parseInputSourceCode();

    
//     return 0;
// }
