#include "comparator.h"

// Open dot file 
// generate tree file

int main(int argc, char**argv)
{
	size_t filesize;
	char *line;
	int temp;

	open_file(argv);

	filesize = get_filesize(fp);

	pool = calloc(filesize/50, sizeof(tree_node*));

	for(temp=0; temp<5; temp++)
	{
		read_line(fp);
		// ignore header
	}

	while((line = read_line(fp))!=NULL)
	{
		// 1 for decl_node; 0 for link node
		if(check_type(line)==1)	// declare node | link node
			decl_n(line);
		else
			link_n(line);
	}

	root = search_pool(1);
	dump_tree(root);

	free(pool);
   return 0;
}

void dump_tree(tree_node *root)
{
	int i;
	fprintf(stdout, "{ node%d ", root->_id);
//	r_sq_brk++;
	for(i=0; i < root->n_child; i++)
	{
		dump_tree(*(root->l_child+i));
	}
	fprintf(stdout, "}");

}

static tree_node *search_pool(int id)
{
	int temp;

//	DEBUG(in search pool);
	for(temp = n_inpool-1; temp>=0; temp--)
	{
		tree_node *cur_node;
		cur_node = *(pool+temp);

		assert(NULL != cur_node);
		if(id == cur_node->_id)
			return cur_node;	
	}
}

void decl_n(char *line)
{
	// ADD to Pool
	tree_node *node = calloc(1, sizeof(tree_node));

	node->nodeN = calloc(16, sizeof(char));
	sscanf(line, "%s", node->nodeN);

	sscanf(node->nodeN+4, "%d", &node->_id);

	node->n_child = 0;
	node->l_child = calloc(1, sizeof(tree_node*));

	*(pool+n_inpool) = node;
	n_inpool++;

	free(line);
}

void link_n(char *line)
{
	// parent & child must both exist
//	DEBUF("%s", line);
	tree_node *parent;
	tree_node *child;
	int parent_id, child_id;
	char temp[64];

	sscanf(line, " node%d", &parent_id);
	sscanf(line, " node%*d%*[^e]e%d", &child_id);

//	DEBUF("Parentid: %d", parent_id);
//	DEBUF("child id: %d", child_id);
	
	// Search pool for parent
	// Search pool for child
	parent = search_pool(parent_id);
	child = search_pool(child_id);
	assert(NULL != parent);
	assert(NULL != child);
	assert(NULL != parent->l_child);

	// Resize parent's child nodes
	parent->n_child++;
	parent->l_child = realloc(parent->l_child, parent->n_child);
	// add child to parent
	*(parent->l_child+parent->n_child-1) = child;
	//child->parent = parent;
}

int check_type(char *line)
{
	//   node4->node5;
	//  node6 [label="call get_filesize",shape=ellipse];
	//  node4 -> node6 [style=dotted]
	int type;
	char temp[128];
	sscanf(line, "%[^;]", temp);
	if(*(temp+strlen(temp)-1) == ']')
		type = 1;
	else type = 0;

	return type;
}

char *read_line(FILE *fileptr)
{
	char *line = calloc(128, sizeof(char));
	if(fgets(line, 128, fileptr)==NULL)
		return NULL;
	if(*line == '}')
	{
		free(line);
		return NULL;
	}
	return line;
}

void
open_file(char**argv)
{
//	char filedir[256] = "../";
//	strcat(filedir, argv[1]);
	char *filedir = argv[1];
        if((fp =fopen(filedir, "r")) == NULL)
        {
                fprintf(stderr, "filedir: %s\n", filedir);
                fprintf(stderr, "Usage: ./comparator <file dir>\n");
                fflush(stderr);
                exit(0);
        }
}

size_t
get_filesize(FILE *fileptr)
{
        // only call this function before you move the ptr to file position
	assert(NULL != fileptr);
        fseek(fileptr, 0L, SEEK_END);

        size_t size =(size_t)ftell(fileptr);
        rewind(fileptr);
        return size;
}

static char
peek(FILE *fileptr)
{
        // peek next
        // eat \n
        int next = fgetc(fileptr);
        ungetc(next, fileptr);

        return (char)next;
}

