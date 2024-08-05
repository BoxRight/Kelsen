#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nl_generation.h"
#include "ast.h"


// Function to collect all assets
void collect_assets(ASTNode *root) {
    if (!root) return;
    
    if (root->type == AST_ASSET_EXPRESSION) {
        collected_assets_count++;
        collected_assets = realloc(collected_assets, collected_assets_count * sizeof(ASTNode *));
        collected_assets[collected_assets_count - 1] = root;
    }
    
    collect_assets(root->left);
    collect_assets(root->right);
    collect_assets(root->next);
}

// Function to collect all subjects
void collect_subjects(ASTNode *root) {
    if (!root) return;
    
    if (root->type == AST_SUBJECT_EXPRESSION) {
        collected_subjects_count++;
        collected_subjects = realloc(collected_subjects, collected_subjects_count * sizeof(ASTNode *));
        collected_subjects[collected_subjects_count - 1] = root;
    }
    
    collect_subjects(root->left);
    collect_subjects(root->right);
    collect_subjects(root->next);
}

// Function to generate inventory of rights
char *generarInventarioDerechos(int n, char **descriptions, char **firstSubjects, char **secondSubjects) {
    char *inventario = (char *)malloc(1024 * n);
    strcpy(inventario, "#### Cláusula X: Inventario de Derechos Personales y Obligaciones de Hacer\n\n");

    for (int i = 0; i < n; i++) {
        char entry[256];
        snprintf(entry, 256, "%d. Derecho a %s\n   - Descripción de la conducta de hacer: %s\n   - Sujeto de Derecho: %s\n   - Sujeto de Obligación: %s\n\n",
                 i + 1, descriptions[i], descriptions[i], firstSubjects[i], secondSubjects[i]);
        strcat(inventario, entry);
    }

    return inventario;
}

// Function to generate subject descriptions
char *generarDescripcionSujetos(int n, char **names, char **addresses, char **emails) {
    char *descripcion = (char *)malloc(1024 * n);
    strcpy(descripcion, "#### Descripción de Sujetos\n\n");

    for (int i = 0; i < n; i++) {
        char entry[256];
        snprintf(entry, 256, "%d. Nombre: \"%s\"\n   - Dirección: \"%s\"\n   - Email: \"%s\"\n\n",
                 i + 1, names[i], addresses[i], emails[i]);
        strcat(descripcion, entry);
    }

    return descripcion;
}

