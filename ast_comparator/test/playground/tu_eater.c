#include "tu_eater.h" 


int main(int argc, char **argv)
{
	size_t filesize;

	open_file(argv);
	filesize = get_filesize();

	eval_file(argv[1]);
}



static node * 
eval_node(void)
{
	// some node occupy more than 1 line
	char line[72];
	node *n = (node *)calloc(1, sizeof(node));
	char next;

	n->_inner = (char *)calloc(INNER_SIZE, 1);
	// read a new line
	if(fgets(line, 72, fp) == NULL)
	       return NULL;	
	// set type, to_do, _inner and id 
	eval_ntype(line, n);

	while(((next = peek()) != EOF)&&(next != '@'))
	{
		memset(line, 0, 72);
		fgets(line, 72, fp);
		strcat(n->_inner, line);
	}
	return n;
}

static void stmt_to_dot(node *n)
{
	// TODO

	printf("TO DOT\n");
	fflush(stdout);
	exit(0);
}

static char  
peek(void)
{
	// peek next
	// eat \n
	int next = fgetc(fp);
	ungetc(next, fp);

	return (char)next;
}			

static void 
stub_to_dot(node *n)
{
	// TODO
	DEBUG("This is a stub func");
}

static void  
eval_ntype(char *buffer, node *n)
{
	// eat the buffer
	// feed the node
	char node_type[32];
	assert(buffer != NULL);
	assert(n != NULL);

	sscanf(buffer, "@%d %s ", &n->_id, node_type);

	strcpy(buffer, buffer + 8 + strlen(node_type));

	n->_ntype = str2node(node_type, n);
	strcpy(n->_inner, buffer);
}

static NODE_TYPE str2node(char *node_type, node *n)
{
	NODE_TYPE _t;
	switch(*node_type)
	{
		case 's':
			_t = statement_list;
			n->to_dot = stmt_to_dot;
			break;
		case 'i':
			if(*(node_type+1) == 'd')
 				_t = identifier_node;
			else if(*(node_type+1) == 'n')
			{
				if(*(node_type + 8) == 'c')
					_t = integer_cst;
				else
					_t = integer_type;
			}
			break;
		case 'p':
			_t = pointer_type;
			break;
		case 'r':
			if(*(node_type+2) == 's')
 				_t = result_decl;
			else if(*(node_type+2) == 't')
				_t = return_expr;
			break;		
		case 'd':
			_t = decl_expr;
			break;
		case 'm':
			_t = modify_expr;
			break;
		case 'v':
			_t = var_decl;
			break;
		case 'f':
			if(*(node_type+9) == 'd')
				_t = function_decl;
			else if(*(node_type+9) == 't')
				_t = function_type;
			break;
		case 't':
			_t = type_decl;
			break;
		case 'c':
			_t = complex_type;
			break;
		case 'a':
			_t = array_type;
			break;
		case 'b':
			if(*(node_type+1) == 'o')
				_t = boolean_type;
			else if(*(node_type+1) == 'i')
				_t = bind_expr;
			break;

		default:
			_t = -1;
			fprintf(stderr, "Unknown node type:%s\n", node_type);
			fflush(stderr);
	}

	// TODO
	if(_t != statement_list)
		n->to_dot = stub_to_dot;
	return _t;
}

static void dump_node(node *n)
{
	assert(n!=NULL);
	printf("ID: %d\ntype: 0x%x\n%s\n", n->_id, n->_ntype, n->_inner);
	fflush(stdout);
}

void 
open_file(char **argv)
{
        if((fp =fopen(argv[1], "r")) == NULL)
        {
                fprintf(stderr, "Usage: ./tu_eater <file dir>\n");
                fflush(stderr);
                exit(0);
        }
}

static int 
check_inner(node *n, char *name)
{
	assert(n != NULL);
	assert(name != NULL);

	if(NULL == strstr(n->_inner, name))
	{
		DEBUF("%s not in ", name);
		DEBUF("%s", n->_inner);
		DEBUG("So return 0");
		return 0;
	}
	else
	{
		DEBUF("%s in ", name);		
		DEBUF("%s", n->_inner);
		DEBUG("So return 1");
		return 1;
	}
}

void eval_statement(node *n)	
{
	// from this node we found our love
	
	node *node_list = (node *)calloc(NUM_NODE, sizeof(node));
	node *temp = calloc(1, sizeof(node));

	dump_node(n);

	while(temp->_ntype != return_expr)
	{
		temp = eval_node();
		
		dump_node(temp);
	}

	printf("\n\n");
	fflush(stdout);
// TODO
	for(int i=0; i < 10; i++)
	{
		temp = eval_node();
		dump_node(temp);
	}
}

size_t 
get_filesize(void)
{
	// only call this function before you move the ptr
        fseek(fp, 0L, SEEK_END);
        size_t size =(size_t)ftell(fp);
        rewind(fp);
	return size;
}

void 
eval_file(char *name)
{
	node *n = (node *)calloc(1, sizeof(node)); 
	// n will be the root of ast
	// statement_list, in our example
	
	while(1)
	{
		n = eval_node();

		if(n == NULL)
		// EOF
			break;
//		dump_node(n);

		if(statement_list == n->_ntype)
		{
			eval_statement(n);
			break;
		}
/*		if(1 == check_inner(n, name))
		{
			eval_statement(n);					
		}
*/
		memset(n, 0, sizeof(node));
	}


        if( fclose(fp) == EOF)
        {
                fprintf(stderr, "File close error:%s\n");
                fflush(stderr);
                exit(0);
        }

	free(n->_inner);
}

