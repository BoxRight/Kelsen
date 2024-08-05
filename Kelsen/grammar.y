%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"

ASTNode *root;  // Define the root variable

extern int yylex(void);
extern int yylineno;
extern char *yytext;

SymbolTable *symbol_table;

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s at line %d, near token '%s'\n", s, yylineno, yytext);
}

#define YYDEBUG 1
%}

%union {
    int ival;
    double fval;
    char *sval;
    struct ASTNode *node;
}

%token <ival> TOKEN_INT_LITERAL
%token <fval> TOKEN_FLOAT_LITERAL
%token <sval> TOKEN_STRING_LITERAL
%token <sval> TOKEN_IDENTIFIER

%token TOKEN_TYPE_INT TOKEN_TYPE_FLOAT TOKEN_TYPE_STRING TOKEN_TYPE_ASSET TOKEN_TYPE_SUBJECT TOKEN_TYPE_CLAUSE TOKEN_TYPE_QUERY
%token TOKEN_ADD TOKEN_SUB TOKEN_PLUS TOKEN_MINUS TOKEN_ASSIGN
%token TOKEN_SEMICOLON TOKEN_COMMA
%token TOKEN_CR_OPEN TOKEN_OB_OPEN TOKEN_PR_OPEN TOKEN_PVG_OPEN
%token TOKEN_CLOSE_PAREN TOKEN_OPEN_BRACE TOKEN_CLOSE_BRACE
%token TOKEN_CONDITION TOKEN_CONSEQUENCE TOKEN_AND
%token TOKEN_PWR_OPEN TOKEN_LIAB_OPEN TOKEN_DIS_OPEN TOKEN_IMM_OPEN
%token TOKEN_UNKNOWN

%token TOKEN_SERVICE TOKEN_PROPERTY TOKEN_NM TOKEN_M TOKEN_CLAUSE

%type <node> program declarations declaration type identifier expression numeric_expression additive_expression string_expression asset_expression subject_expression clause_expression legal_expression condition_expression consequence_expression conditional_expression asset_type asset_subtype
%type <node> claim_right obligation prohibition privilege power liability disability immunity

%left TOKEN_ADD TOKEN_SUB
%left TOKEN_AND

%%

program:
      declarations { root = $1; }
    ;

declarations:
      declaration { $$ = $1; }
    | declarations declaration { $$ = create_declarations($1, $2); }
    ;

declaration:
      type identifier TOKEN_ASSIGN expression TOKEN_SEMICOLON { $$ = create_declaration($1, $2, $4); }
    | type identifier TOKEN_SEMICOLON { $$ = create_declaration($1, $2, NULL); }
    ;

type:
      TOKEN_TYPE_INT { $$ = create_type_node("int"); }
    | TOKEN_TYPE_FLOAT { $$ = create_type_node("float"); }
    | TOKEN_TYPE_STRING { $$ = create_type_node("string"); }
    | TOKEN_TYPE_ASSET { $$ = create_type_node("asset"); }
    | TOKEN_TYPE_SUBJECT { $$ = create_type_node("subject"); }
    | TOKEN_TYPE_CLAUSE { $$ = create_type_node("clause"); }
    | TOKEN_TYPE_QUERY { $$ = create_type_node("query"); }
    ;

identifier:
      TOKEN_IDENTIFIER { $$ = create_identifier($1); }
    ;

expression:
      numeric_expression { $$ = $1; }
    | additive_expression { $$ = $1; }
    | asset_expression { $$ = $1; }
    | subject_expression { $$ = $1; }
    | string_expression { $$ = $1; }
    | legal_expression { $$ = $1; }
    | conditional_expression { $$ = $1; }
    | clause_expression { $$ = $1; }
    ;

numeric_expression:
      TOKEN_INT_LITERAL { $$ = create_number($1); }
    | TOKEN_FLOAT_LITERAL { $$ = create_float($1); }
    ;

additive_expression:
      numeric_expression TOKEN_ADD numeric_expression { $$ = create_binary_op(AST_ADD, $1, $3); }
    | numeric_expression TOKEN_SUB numeric_expression { $$ = create_binary_op(AST_SUB, $1, $3); }
    ;

string_expression:
      TOKEN_STRING_LITERAL { $$ = create_string($1); }
    ;

asset_expression:
      asset_type TOKEN_COMMA asset_subtype TOKEN_COMMA subject_expression TOKEN_COMMA string_expression TOKEN_COMMA subject_expression
        { $$ = create_asset_expression($1, $3, $5, $7, $9); }
    | asset_type TOKEN_COMMA asset_subtype TOKEN_COMMA identifier TOKEN_COMMA identifier TOKEN_COMMA identifier
        { $$ = create_asset_expression($1, $3, $5, $7, $9); }
    ;

subject_expression:
      string_expression TOKEN_COMMA string_expression TOKEN_COMMA TOKEN_INT_LITERAL TOKEN_COMMA string_expression
        { $$ = create_subject_expression($1, $3, $5, $7); }
    ;


legal_expression:
      claim_right { $$ = $1; }
    | obligation { $$ = $1; }
    | prohibition { $$ = $1; }
    | privilege { $$ = $1; }
    | power { $$ = $1; }
    | liability { $$ = $1; }
    | disability { $$ = $1; }
    | immunity { $$ = $1; }
    ;

claim_right:
      TOKEN_CR_OPEN asset_expression TOKEN_CLOSE_PAREN { $$ = create_legal(AST_CR, $2); }
    | TOKEN_CR_OPEN identifier TOKEN_CLOSE_PAREN { $$ = create_legal(AST_CR, $2); }
    ;

obligation:
      TOKEN_OB_OPEN asset_expression TOKEN_CLOSE_PAREN { $$ = create_legal(AST_OB, $2); }
    | TOKEN_OB_OPEN identifier TOKEN_CLOSE_PAREN { $$ = create_legal(AST_OB, $2); }
    ;

prohibition:
      TOKEN_PR_OPEN asset_expression TOKEN_CLOSE_PAREN { $$ = create_legal(AST_PR, $2); }
    | TOKEN_PR_OPEN identifier TOKEN_CLOSE_PAREN { $$ = create_legal(AST_PR, $2); }
    ;

privilege:
      TOKEN_PVG_OPEN asset_expression TOKEN_CLOSE_PAREN { $$ = create_legal(AST_PVG, $2); }
    | TOKEN_PVG_OPEN identifier TOKEN_CLOSE_PAREN { $$ = create_legal(AST_PVG, $2); }
    ;

condition_expression:
      TOKEN_CONDITION asset_expression { $$ = create_condition($2); }
    | TOKEN_CONDITION identifier { $$ = create_condition($2); }
    | TOKEN_CONDITION asset_expression TOKEN_AND asset_expression
        { $$ = create_condition(create_binary_op(AST_AND, $2, $4)); }
    | TOKEN_CONDITION identifier TOKEN_AND identifier
        { $$ = create_condition(create_binary_op(AST_AND, $2, $4)); }
    ;

consequence_expression:
      TOKEN_CONSEQUENCE legal_expression { $$ = create_consequence($2); }
    | TOKEN_CONSEQUENCE identifier { $$ = create_consequence($2); }
    ;
    
conditional_expression:
      condition_expression consequence_expression { $$ = create_conditional($1, $2); }
    ;

clause_expression:
      TOKEN_OPEN_BRACE conditional_expression TOKEN_CLOSE_BRACE
        { $$ = create_clause_expression($2); }
    ;

power:
      TOKEN_PWR_OPEN identifier TOKEN_COMMA identifier TOKEN_COMMA legal_expression TOKEN_CLOSE_PAREN
        { $$ = create_legal(AST_PWR, create_binary_op(AST_COMMA, $2, create_binary_op(AST_COMMA, $4, $6))); }
    ;


liability:
      TOKEN_LIAB_OPEN identifier TOKEN_COMMA identifier TOKEN_COMMA legal_expression TOKEN_CLOSE_PAREN
        { $$ = create_legal(AST_LIAB, create_binary_op(AST_COMMA, $2, create_binary_op(AST_COMMA, $4, $6))); }
    ;

disability:
      TOKEN_DIS_OPEN identifier TOKEN_COMMA identifier TOKEN_COMMA legal_expression TOKEN_CLOSE_PAREN
        { $$ = create_legal(AST_DIS, create_binary_op(AST_COMMA, $2, create_binary_op(AST_COMMA, $4, $6))); }
    ;

immunity:
      TOKEN_IMM_OPEN identifier TOKEN_COMMA identifier TOKEN_COMMA legal_expression TOKEN_CLOSE_PAREN
        { $$ = create_legal(AST_IMM, create_binary_op(AST_COMMA, $2, create_binary_op(AST_COMMA, $4, $6))); }
    ;
    
asset_type:
      TOKEN_SERVICE { $$ = create_type_node("Service"); }
    | TOKEN_PROPERTY { $$ = create_type_node("Property"); }
    ;

asset_subtype:
      TOKEN_NM { $$ = create_type_node("NM"); }
    | TOKEN_M { $$ = create_type_node("M"); }
    | TOKEN_PLUS { $$ = create_type_node("+"); }
    | TOKEN_MINUS { $$ = create_type_node("-"); }
    ;

%%


