#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "parser.tab.h"  // Include the parser header
#include "symbol_table.h"
#include "globals.h"

extern SymbolTable *symbol_table;
extern ASTNode *root;

extern FILE *yyin;


int main(int argc, char **argv) {
    // Initialize the symbol table
    symbol_table = create_symbol_table();

    // Open the input file
    if (argc > 1) {
        FILE *input = fopen(argv[1], "r");
        if (input) {
            yyin = input;
        } else {
            perror(argv[1]);
            return 1;
        }
    }

    // Parse the input
    yyparse();

    // Print the AST
    printf("Abstract Syntax Tree:\n");
    print_ast(root, 0);
    

    // Print the symbol table after parsing
    print_symbol_table(symbol_table);
    free_ast(root);
    // Free the symbol table
    free_symbol_table(symbol_table);

    return 0;
}

