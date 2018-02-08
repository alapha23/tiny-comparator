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

	cur_all_dot("eval");

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
	char scope[64];

	while(1)
	{
		if(NULL == readline(line, 128, fp))
		{
			// EOF
			break;
		}
		if(is_scope(line))	
		{
			sscanf("SCOPE: %s", scope);
		}
		else
		{
			// Add a new node
			if(add_to_node(line) == NULL)
				break;	
			//dump_node(n);
		}
	}
}

node *add_to_node(char *line)
{
	// has myname existed in pool?
	char *myname = calloc(64, sizeof(char));
	node *n;
	plagiarism *p;

	assert(NULL != line);
	if(*line == ' ')
		return NULL;

	sscanf(line, "| %s ", myname);
	
	n = search_pool(myname);
	if(n == NULL)
	{
		// not exist, build a new node
		n = (node *)calloc(1, sizeof(node));
		// add new node to pool
		*(pool+n_inpool) = n;
		n->id = n_inpool;
		strcpy(n->filename, myname);
		n_inpool++;
	}

	// ignore plagiarism lower than 40%
	float match;
	sscanf(line, "| %*s | %*s | %f | %*s ", &match);
	match = match *100;
//	DEBUF("match: %.2f", match);
//	DEBUF("Threshold %d", Tolerance);
	if(match > (float)Tolerance)
	{
		// build a new plagiarism relationship
		n->n_plagiarism++;

		p = calloc(1, sizeof(plagiarism));
		sscanf(line, "| %*s | %s | %f | %s ", p->hername, &p->match, p->scope);
		p->target_id = n->n_plagiarism;
	       	p->hertype = check_type(p->hername);

		// add plagiarsm
		assert(n->n_plagiarism != N_REF);
		n->plag[n->n_plagiarism-1] = p;

		free(myname);
		return n;
	}
	else
		return n;
}

int check_type(char *hername)
{
	if(*(hername) == 'r')
	{
	// reference
		return 2;
	}else if (*(hername) == 'p')
	{
		return 1;
	}
	else
		return 0;
}

node *search_pool(char *myname)
{
	node *ret = NULL;
	int i = n_inpool;
	while(i)
	{
		ret = *(pool + i - 1);
//		printf("myname:%s vs pool:%s\n", myname, ret->filename);
//		fflush(stdout);
		if(!strcmp(myname, ret->filename))
			return ret;
		i--;
	}
	return NULL;
}

void dump_node(node *n)
{
	int i = n->n_plagiarism;
	plagiarism *p;
	DEBUF("node id: %d", n->id);
	DEBUF("my name: %s", n->filename);
	DEBUF("Num of plagiarisms: %d", n->n_plagiarism);
	while(i)
	{
		p = n->plag[i-1];
		DEBUF("Plagiarism id: %d", p->target_id);
		DEBUF("hername : %s", p->hername);
		DEBUF("Hertype: %d", p->hertype);
		DEBUF("scope: %s", p->scope);
		DEBUF("match: %f", p->match);
		i--;
	}
}

void cur_all_dot(char *scope)
{
	int iter = n_inpool-1;
	node *n;
	plagiarism *p;
	char *id1 = calloc(32, sizeof(char));
	char *id2 = calloc(32, sizeof(char));
	char link_label[64];
	emit_header(scope, "cur vs. prev");
	while(iter)
	{
		n = *(pool+iter-1);
		int i = n->n_plagiarism;
		sscanf(n->filename, "%[^.]%*s", id1);
		dot_box(id1, n->filename);
		while(i)
		{
			p = n->plag[i-1];
			// check if scope meets condition
			if(strcmp(scope,p->scope))
			{
//				fprintf(stdout,"Not this scope: %s compared to %s\n", (n->plag)[i-1]->scope, scope);

				continue;
			}
			// plagiarism dots
			sscanf((n->plag)[i-1]->hername, "%[^.]", id2);
			dot_ellipse(id2, (n->plag)[i-1]->hername);
			sprintf(link_label, "%s: %.1f\%", scope, (n->plag)[i-1]->match*100.0);
			dot_link(id1, id2, link_label);
			i--;
		}
		iter--;
	}
	emit_footer();
	free(id1);
	free(id2);
}
