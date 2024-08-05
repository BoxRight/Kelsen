#include "globals.h"
#include "symbol_table.h" // Include the header that defines SymbolTable
#include <stddef.h>



ASTNode **collected_assets = NULL;
int collected_assets_count = 0;

ASTNode **collected_subjects = NULL;
int collected_subjects_count = 0;

SymbolTable *symbol_table = NULL;
