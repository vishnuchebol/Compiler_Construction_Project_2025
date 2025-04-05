#ifndef PARSER
#define PARSER
#include "parserDef.h"

int getNonterminalIndex(const char* nt);
int getTerminalIndex(const char* t);
void readGrammar(char* filename);
void computeFirstSets();
void printFirstSets();
void computeFollowSets();
void printFollowSets();
void computeParseTable();
void printParseTable();
ParseTreeNode* createNode(const char* symbol);
ParseTreeNode* parseInputSourceCode();
void initParser(char * filename);
#endif // !PARSER