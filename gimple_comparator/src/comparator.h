#ifndef __COMP
#define __COMP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXPR_TYPE int

#define decl_expr 	0x001
#define modify_expr 	0x002
#define return_expr	0x003

#define var_decl
#define function_decl
#define identifier_node
#define 

void open_file(const char *name);

void emit_modify_expr(void);

void read_symboltable(void);

typedef struct 
{
	int _id;
	EXPR_TYPE _expr_type;
	EXPR_TYPE _
} node;

#endif
