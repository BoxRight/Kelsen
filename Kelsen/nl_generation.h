#ifndef NL_GENERATION_H
#define NL_GENERATION_H

#include "ast.h"

void collect_assets(ASTNode *root);
void collect_subjects(ASTNode *root);
char *generarInventarioDerechos(int n, char **descriptions, char **firstSubjects, char **secondSubjects);
char *generarDescripcionSujetos(int n, char **names, char **addresses, char **emails);

#endif // NL_GENERATION_H

