#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode *create_number(int value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;
    node->var_type = TYPE_INT;
    node->data.ival = value;
    node->data.strval = malloc(20);  // Allocate memory for the string representation
    sprintf(node->data.strval, "%d", value);
    node->next = NULL;
    return node;
}

ASTNode *create_string(char *value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = AST_STRING;
    node->var_type = TYPE_STRING;
    node->data.sval = strdup(value);
    node->data.strval = strdup(value);
    node->next = NULL;
    return node;
}

ASTNode *create_identifier(char *name) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = AST_IDENTIFIER;
    node->var_type = TYPE_UNKNOWN;
    node->data.sval = strdup(name);
    node->data.strval = strdup(name);
    node->next = NULL;
    return node;
}

ASTNode *create_float(double value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = AST_FLOAT;
    node->var_type = TYPE_FLOAT;
    node->data.fval = value;
    node->next = NULL;
    return node;
}

ASTNode *create_type_node(const char *type_name) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = AST_TYPE;
    node->data.sval = strdup(type_name);
    
    if (strcmp(type_name, "int") == 0) {
        node->var_type = TYPE_INT;
    } else if (strcmp(type_name, "float") == 0) {
        node->var_type = TYPE_FLOAT;
    } else if (strcmp(type_name, "string") == 0) {
        node->var_type = TYPE_STRING;
    } else if (strcmp(type_name, "asset") == 0) {
        node->var_type = TYPE_ASSET;
    } else if (strcmp(type_name, "subject") == 0) {
        node->var_type = TYPE_SUBJECT;
    } else if (strcmp(type_name, "clause") == 0) {
        node->var_type = TYPE_CLAUSE;
    } else if (strcmp(type_name, "query") == 0) {
        node->var_type = TYPE_QUERY;
    } else {
        node->var_type = TYPE_UNKNOWN;
    }

    return node;
}


ASTNode *create_binary_op(ASTNodeType type, ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    node->var_type = TYPE_UNKNOWN;
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;
    node->next = NULL;
    return node;
}

ASTNode *create_clause_expression(ASTNode *condition, ASTNode *consequence) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = AST_CLAUSE_EXPRESSION;
    node->var_type = TYPE_CLAUSE;
    node->data.clause.condition = condition;
    node->data.clause.consequence = consequence;
    node->data.strval = malloc(strlen(condition->data.strval) + strlen(consequence->data.strval) + 50);
    sprintf(node->data.strval, "condition: %s consequence: %s", condition->data.strval, consequence->data.strval);
    node->next = NULL;
    return node;
}

ASTNode *create_legal(ASTNodeType type, ASTNode *expr) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    node->data.legal.expression = expr;
    switch (type) {
        case AST_CR:
            node->var_type = TYPE_CLAIM_RIGHT;
            break;
        case AST_OB:
            node->var_type = TYPE_OBLIGATION;
            break;
        case AST_PR:
            node->var_type = TYPE_PROHIBITION;
            break;
        case AST_PVG:
            node->var_type = TYPE_PRIVILEGE;
            break;
        case AST_PWR:
            node->var_type = TYPE_POWER;
            break;
        case AST_LIAB:
            node->var_type = TYPE_LIABILITY;
            break;
        case AST_DIS:
            node->var_type = TYPE_DISABILITY;
            break;
        case AST_IMM:
            node->var_type = TYPE_IMMUNITY;
            break;
        default:
            node->var_type = TYPE_UNKNOWN;
            break;
    }
    return node;
}



ASTNode *create_condition(ASTNode *expr) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = AST_CONDITION;
    node->data.legal.expression = expr;
    node->next = NULL;
    return node;
}

ASTNode *create_consequence(ASTNode *expr) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = AST_CONDITION; // Use the correct type if different
    node->data.legal.expression = expr;
    node->next = NULL;
    return node;
}

ASTNode *create_asset_expression(ASTNode *type, ASTNode *subtype, ASTNode *subject1, ASTNode *description, ASTNode *subject2) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = AST_ASSET_EXPRESSION;
    node->var_type = TYPE_ASSET;
    node->data.asset.type = type;
    node->data.asset.subtype = subtype;
    node->data.asset.subject1 = subject1;
    node->data.asset.description = description;
    node->data.asset.subject2 = subject2;
    node->next = NULL;
    return node;
}

ASTNode *create_subject_expression(ASTNode *description1, ASTNode *description2, int age, ASTNode *location) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = AST_SUBJECT_EXPRESSION;
    node->var_type = TYPE_SUBJECT;
    node->data.subject.description1 = description1;
    node->data.subject.description2 = description2;
    node->data.subject.age = age;
    node->data.subject.location = location;
    node->next = NULL;
    return node;
}

ASTNode *create_declarations(ASTNode *declarations, ASTNode *declaration) {
    if (!declarations) return declaration;

    ASTNode *node = declarations;
    while (node->next) {
        node = node->next;
    }
    node->next = declaration;
    return declarations;
}

ASTNode *create_declaration(ASTNode *type, ASTNode *identifier, ASTNode *expression) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = AST_DECLARATION;
    node->data.declaration.type = type;
    node->data.declaration.identifier = identifier;
    node->data.declaration.expression = expression;
    node->next = NULL;
    return node;
}

void print_ast(ASTNode *node, int level) {
    if (!node) return;

    for (int i = 0; i < level; i++) {
        printf("  "); // Indentation
    }

    switch (node->type) {
        case AST_NUMBER:
            printf("Number: %d\n", node->data.ival);
            break;
        case AST_FLOAT:
            printf("Float: %f\n", node->data.fval);
            break;
        case AST_STRING:
            printf("String: %s\n", node->data.sval);
            break;
        case AST_IDENTIFIER:
            printf("Identifier: %s\n", node->data.sval);
            break;
        case AST_TYPE:
            printf("Type: %s\n", node->data.sval);
            break;
        case AST_ADD:
            printf("Add:\n");
            break;
        case AST_SUB:
            printf("Sub:\n");
            break;
        case AST_CR:
            printf("Claim Right:\n");
            break;
        case AST_OB:
            printf("Obligation:\n");
            break;
        case AST_PR:
            printf("Prohibition:\n");
            break;
        case AST_PVG:
            printf("Privilege:\n");
            break;
        case AST_PWR:
            printf("Power:\n");
            break;
        case AST_LIAB:
            printf("Liability:\n");
            break;
        case AST_DIS:
            printf("Disability:\n");
            break;
        case AST_IMM:
            printf("Immunity:\n");
            break;
        case AST_AND:
            printf("And:\n");
            break;
        case AST_CONDITION:
            printf("Condition:\n");
            break;
        case AST_CLAUSE_EXPRESSION:
            printf("Clause Expression:\n");
            print_ast(node->data.clause.condition, level + 1);
            print_ast(node->data.clause.consequence, level + 1);
            break;
        case AST_ASSET_EXPRESSION:
            printf("Asset Expression:\n");
            break;
        case AST_SUBJECT_EXPRESSION:
            printf("Subject Expression:\n");
            break;
        case AST_QUERY:
            printf("Query:\n");
            break;
        case AST_DECLARATIONS:
            printf("Declarations:\n");
            break;
        case AST_DECLARATION:
            printf("Declaration:\n");
            break;
        case AST_BINARY_OP:
            printf("Binary Operation:\n");
            print_ast(node->data.binary_op.left, level + 1);
            print_ast(node->data.binary_op.right, level + 1);
            break;
        default:
            printf("Unknown AST Node Type: %d\n", node->type);
    }

    print_ast(node->next, level);
}

void free_ast(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case AST_NUMBER:
        case AST_FLOAT:

            break;
        case AST_STRING:
        case AST_IDENTIFIER:
        case AST_TYPE:
            free(node->data.sval);
            free(node->data.strval);
            break;
        case AST_ADD:
        case AST_SUB:
        case AST_AND:
        case AST_BINARY_OP:
        case AST_CLAUSE_EXPRESSION:
        case AST_SUBJECT_EXPRESSION:
        case AST_ASSET_EXPRESSION:
        case AST_DECLARATION:
        case AST_QUERY:
        case AST_CONDITION:
        case AST_CR:
        case AST_OB:
        case AST_PR:
        case AST_PVG:
        case AST_PWR:
        case AST_LIAB:
        case AST_DIS:
        case AST_IMM:
            free_ast(node->data.binary_op.left);
            free_ast(node->data.binary_op.right);
            break;
        case AST_DECLARATIONS:
            free_ast(node->data.declaration.type);
            free_ast(node->data.declaration.identifier);
            free_ast(node->data.declaration.expression);
            break;
        default:
            break;
    }

    free(node);
}

