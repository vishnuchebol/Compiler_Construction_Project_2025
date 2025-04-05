#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lexer.h"   // For close(), read(), write()
#include <sys/types.h> // For data types like off_t
#include <sys/stat.h>  // For file permissions
#include "parser.h"
//#include "parser.h"
#include "interface.h"
#include "parserDef.h"

const char* TokenNameStrings[] = {
    "TK_UNKNOWN",
    "TK_ASSIGNOP",
    "TK_COMMENT",
    "TK_ID",
    "TK_RUID",
    "TK_UNION",
    "TK_ENDUNION",
    "TK_DEFINETYPE",
    "TK_AS",
    "TK_NUM",
    "TK_RNUM",
    "TK_FIELDID",
    "TK_FUNID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_AND",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE",
    "TK_ERR",
    "TK_EPS",
    "TK_DOLLAR"
};
const char *getTokenNameString(TokenName token) {
    return TokenNameStrings[token];
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }
    FILE* fp;
    printf("Implementation Status: Both lexical and syntax analysis modules implemented.\n");
    
    int choice;
    //initializeLexer();
    while (1) {
        printf("\nOptions:\n");
        printf("0. Exit\n");
        printf("1. Remove comments\n");
        printf("2. Print token list\n");
        printf("3. Parse code and print parse tree\n");
        printf("4. Measure execution time\n");
        // printf("Enter your choice: ");
        scanf("%d", &choice);
        
        //printf("HERE!!!");
        switch (choice) {
            case 0:
                return 0;
            case 1:
                //printf("HERE!!!\n");
                fp = fopen(argv[1],"r");
                initializeLexer(fp);
                char cleanFile[256];
                sprintf(cleanFile, argv[2], argv[1]);
                
                removeComments(argv[1], cleanFile);
                printf("Comments removed and saved to %s\n", argv[2]);
                break;
            case 2:
                fp = fopen(argv[1],"r");
                initializeLexer(fp);
                if (fp == NULL) {
                    printf("Could not open file %s\n", argv[1]);
                    break;
                }
                // printf("HERE!!!!!!!!!!!!!! \n");
                // getStream(fp,B);
                // printf("HERE!!!!!!!!!!!!!! \n");
                tokenInfo token;
                printf("Token List:\n");
                //printf("%-15s %-15s %-10s\n", "Token Name", "Lexeme", "Line Number");
                while (1) {
                    //printf("HERE!!\n");
                    token = getNextToken();
                    if(token.TOKEN_NAME == TK_ERR){
                        printf("Line number: %d Unknown  symbol <%s>\n",token.LINE_NO,token.LEXEME);
                    }
                    if(token.TOKEN_NAME == TK_UNKNOWN){
                        break;
                    }
                    printf("Line Number: %-10d Lexeme: %-15s Token: %-15s\n",  token.LINE_NO,token.LEXEME,getTokenNameString(token.TOKEN_NAME));
                }
                fclose(fp);
                break;
            
            /*case 3:
                // Initialize the parser and lexer
                printf("hi");

                int fd = open(argv[1], O_RDONLY);
                ParsingTable* pTable = initialiseParsingTable();
                printf("hi");
                FirstAndFollow* fafl = computeFirstAndFollowSets(extractGrammar());
                createParseTable(fafl, pTable);
            
                // Parse the input source code
                ParseTree* pt = parseInputSourceCode(argv[1], pTable, fafl);
            
                // Print the parse tree to the specified output file
                FILE* outputFile = fopen(argv[2], "w");
                if (outputFile == NULL) {
                    printf("Error opening output file %s\n", argv[2]);
                    break;
                }
            
                // Assuming you have a function to print the parse tree
                printParseTree(pt, outputFile);
            
                fclose(outputFile);
            
                // Check for errors
                // if (lexicalErrorFlag || syntaxErrorFlag) {
                //     printf("Errors encountered during parsing.\n");
                // } else {
                //     printf("Parsing successful.\n");
                // }
            
                break;
            */
            // case 3:
            //     printf("Commencing parsing of input source code \n");
            //     printf("--------\n");
                //initLexer(fp);

            //     
            //     printf("HERE\n");

            //     FirstAndFollow* fafl = computeFirstAndFollowSets(g);
            //     //printf("Commencin\n");

            //     ParsingTable* pTable = initialiseParsingTable();
            //     //printf("Commencin88888\n");

            //     createParseTable(fafl,pTable);
            //     //printf("Commencin88887788\n");

            //     // Lexer and parser are both invoked inside parseInputSourceCode
            //     ParseTree* pt = parseInputSourceCode(argv[1],pTable,fafl);
            //     printf("Commencin88887788444\n");

            //     printParseTree(pt,NULL);

            //     printf("\nFinished parsing of input source code \n");
            //     break;
            // // case 4:
            // //     clock_t start_time = clock();
            
            // //     // Call the parser function here (assuming it's named parseInputSourceCode)
            // //     // This will invoke both lexer and parser.
            // //     FILE* fp = fopen(argv[1], "r");
            // //     if (fp == NULL) {
            // //         printf("Could not open file %s\n", argv[1]);
            // //         break;
            // //     }
            
            // //     twinBuffer B;
            //     getStream(fp);
            
            //     // Assuming you have a parser function named parseInputSourceCode
            //     // that takes the input file and output file names as arguments.
            //     // Adjust the function name and parameters according to your parser implementation.
            //     parseInputSourceCode(argv[1], argv[2]);
            
            //     fclose(fp);
            
            //     clock_t end_time = clock();
            
            //     double total_CPU_time = (double)(end_time - start_time);
            //     double total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            
            //     printf("Total CPU Time: %f\n", total_CPU_time);
            //     printf("Total CPU Time in Seconds: %f\n", total_CPU_time_in_seconds);
            
            //     break;
            case 3: 
                fp = fopen(argv[1],"r");
                initializeLexer(fp);
                initParser("grammar.txt");
                while(1){
                    token = getNextToken();
                    if(token.TOKEN_NAME == TK_ERR){
                        printf("Line number: %d Unknown  symbol <%s>\n",token.LINE_NO,token.LEXEME);
                    }
                    if(token.TOKEN_NAME == TK_UNKNOWN){
                        break;
                    }
                }
                break;
            case 4:
                fp = fopen(argv[1],"r");
                initializeLexer(fp);
                clock_t start_time, end_time;

                double total_CPU_time, total_CPU_time_in_seconds;

                start_time = clock();            
                // Call the parser function here (assuming it's named parseInputSourceCode)
                // This will invoke both lexer and parser.
                FILE* fp = fopen(argv[1], "r");
                if (fp == NULL) {
                    printf("Could not open file %s\n", argv[1]);
                    break;
                }
                
                // invoke your lexer and parser here
                initParser("grammar.txt");
                initializeLexer(fp);
                end_time = clock();

                total_CPU_time = (double) (end_time - start_time);

                total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
                
                printf("Total CPU Time: %f\n", total_CPU_time);
                printf("Total CPU Time in Seconds: %f\n", total_CPU_time_in_seconds);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}