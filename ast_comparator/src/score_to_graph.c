#include "score_to_graph.h"

/* Open score files, collect plagiarism results & possibility
 *
 *
 * **/

int main(int argc, char **argv)
{
	size_t filesize;
	
	open_file(argv);
	filesize = get_filesize();

	pool = calloc(N_CUR, sizeof(node*));
	
	fill_pool();

	fclose(fp);
	free(pool);
	return 0;
}

void open_file(char **argv)
{
	if((fp = fopen(argv[1], "r")) == NULL)
	{
		DEBUF("Filedir: %s", argv[1]);
		DEBUG("Usage: ./score_to_graph <file dir> ");
		exit(0);	
	}
}

size_t get_filesize(void)
{
	fseek(fp, 0L, SEEK_END);
	size_t size = (size_t)ftell(fp);
	rewind(fp);
	return size;
}

void emit_header(char *scope, char *whovswho)
{
        fprintf(stdout, "digraph AST {\n");
        fprintf(stdout, "  graph [fontname=\"Times New Roman\",fontsize=10];\n");
        fprintf(stdout, "  node  [fontname=\"Courier New\",fontsize=10];\n");
        fprintf(stdout, "  edge  [fontname=\"Times New Roman\",fontsize=10];\n\n");
        
        fprintf(stdout, "  node0 [label=\"%s  %s\",shape=box];\n", scope, whovswho);
        fflush(stdout);
}

void fill_pool(void)
{
	char line[128];

	readline(line, 128, fp);

	DEBUF("%s", line);

}
