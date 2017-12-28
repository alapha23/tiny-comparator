#include "comparator.h"

/*
 * open one file
 * build ast
 * write to file.ast
 *
 * */
/*
 * C expressions are sorted into groups
 'x' for an exceptional code (fits no category).
 't' for a type object code.
 'b' for a lexical block.
 'c' for codes for constants.
 'd' for codes for declarations (also serving as variable refs).
 'r' for codes for references to storage.
 '<' for codes for comparison expressions.
 '1' for codes for unary arithmetic expressions.
 '2' for codes for binary arithmetic expressions.
 's' for codes for "statement" expressions, which have side-effects,
     but usually no interesting value.
 'e' for codes for other kinds of expressions.  
 */

FILE *fp;


int main(void)
{


   return 0;
}

void emit_modify_expr(void)
{

}

void read_symboltable(void)
{


}


void open_file(const char *name)
{
	if(fopen(name, r) == NULL)
	{
		fprintf(stderr, "Cannot open file%s\n", name);
		exit(0);
	}
}
