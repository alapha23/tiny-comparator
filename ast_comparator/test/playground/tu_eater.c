#include "tu_eater.h" 


int main(int argc, char **argv)
{
	size_t filesize;

	open_file(argv);
	filesize = get_filesize();
	
	pool = (node **)calloc(filesize/70, sizeof(node*));
	eval_file(argv[1]);
	free(pool);
}


/*node* search_pool(int id, node **pool, int n)
{
                int i;
                for(i=0; i<n; i++)
                {
                        if(id == (get_by_num(i, pool, n)->_id))
                                break;
			node *temp = get_by_num(i, pool, n);
                }
                return get_by_num(i, pool, n);
}*/ 
/*
 *
 *	temp = search_pool(1845, pool, n_inpool);
	DEBUF("%p", temp->to_dot);
	DEBUF("%s", temp->_inner);
	assert(temp->to_dot != NULL);
	temp->to_dot(temp);
	dump_node(temp);

 *
 * **/

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
	// next line belongs to this node unless EOF or @
	{
		memset(line, 0, 72);
		fgets(line, 72, fp);
		strcat(n->_inner, line);
	}
	ADD2POOL(n, pool, n_inpool);
	if(peek() == EOF)
		return NULL;

	return n;
}

static void var_decl_to_dot(node *n)
{
	int name_i;
	node *name_n;
	char name_c[32];
	sscanf(n->_inner, " name: @%d ", &name_i);
	name_n = search_pool(name_i, pool, n_inpool);
	
	sscanf(name_n->_inner, " strg: %s ", name_c);

	dot_shape(n->_id, name_c);
	dot_link(n->prev->_id, n->_id);
}

static void integer_cst_to_dot(node *n)
{
	int value;
	char *buffer = calloc(1, 6);
	sscanf(n->_inner, " %*s %*s int: %d", &value);
	sprintf(buffer, "%d", value);
	dot_shape(n->_id, buffer);
	dot_link(n->prev->_id, n->_id);
}

static void modify_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	
	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	dot_shape(n->_id, "modify");
	// connect with the previous node
	dot_link_dt(n->prev->_id, n->_id);
}

static void ret_to_dot(node *n)
{
	int expr_id;
	node *expr;

	dot_shape(n->_id, "return");

	sscanf(n->_inner, " %*s %*s expr: @%d", &expr_id);
	expr = search_pool(expr_id, pool, n_inpool);

	expr->prev = n;
	expr->to_dot(expr);	

	dot_link_dt(n->prev->_id, n->_id);
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
	DEBUG(Stub);
	DEBUF("%s", n->_inner);
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
			break;
		case 'i':
			if(*(node_type+1) == 'd')
 				_t = identifier_node;
			else if(*(node_type+1) == 'n')
			{
				if(*(node_type + 8) == 'c')
				{
					_t = integer_cst;
					n->to_dot = integer_cst_to_dot;
				}
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
			{
				_t = return_expr;
				n->to_dot = ret_to_dot;
			}
			break;		
		case 'd':
			_t = decl_expr;
			break;
		case 'm':
			_t = modify_expr;
			n->to_dot = modify_to_dot;
			break;
		case 'v':
			_t = var_decl;
			n->to_dot = var_decl_to_dot;
			break;
		case 'f':
			if(*(node_type+9) == 'd')
				_t = function_decl;
			else if(*(node_type+9) == 't')
				_t = function_type;
			break;
		case 't':
			if(*(node_type+1) == 'r')
				_t = tree_list;
			else if(*(node_type+9) == 'y')
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
	if(n->to_dot == NULL)
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
		return 0;
	}
	else
	{
		return 1;
	}
}

static node** read_statement(node *n)
{
	// n is the identifier node of the scope

	node **node_list = (node **)calloc(NUM_NODE, sizeof(node*));
	node *temp = n;
	int counter = 1;

	assert(NULL != temp);

	// we make first element in the list be the statement_list node
	while(temp->_ntype != statement_list)
	{
		temp = get_next(temp, pool, n_inpool);
	}
	*node_list = temp;	
	while(temp->_ntype != return_expr)
	{
		temp = get_next(temp, pool, n_inpool);
		*(node_list + counter) = temp;
		counter++;
	}

	// DEBUG
	return node_list;
}

static node **parse_stmt(node **node_list)
{
	// first parse the statement
	int num_node;
	int expr[NUM_EXPR];	// we assume there is NUM_EXPR=1024 expr
	int counter = 0;
	node *statement = *node_list;	// first stmt in the list is the statement_list
	char *inner = statement->_inner;

	assert(NULL != statement);
	assert(NULL != node_list);
	while(1)
	{
		if(*(inner + counter) == '\0')
			break;
		if(*(inner+counter) == '@')
		{
			sscanf(inner + counter - 6, "%d ", &num_node);
			sscanf(inner + counter + 1, "%d", &expr[num_node]);
		}
		counter++;	
	}

	node **expr_list = (node **)calloc(num_node, sizeof(node));
	// find the node from the node_list and add to the expr_list
	counter = 1;
	int nelms = 1;
	node *temp = *(node_list+counter);
	do
	{
		for(int i=0; i<= num_node; i++)
		{
			if(temp->_id == expr[i])
			{
				// Add local decl to local symboltable
				// TODO
				if(temp->_ntype == decl_expr)
					;
				else
				{
					*(expr_list+nelms) = temp;
					// set prev node
					(**(expr_list+nelms)).prev = *(expr_list+nelms-1);
					nelms++;			
				}
			}
		}
		counter++;
		temp = *(node_list+counter);
	}while(temp != NULL);
	// the first node in expr_list points to the statement_list
	*expr_list = statement;

	// there are dependencies in the nodes
	// we are expecting to feed the dependencies
	// however, it is not necessary to feed all the dependencies
		

	return expr_list; 
}

static void 
dump_list(node **node_list, char* scope, node *start_node)
{
	node *temp;
	node **expr_list = parse_stmt(node_list);	
	int counter = 1;
	assert(expr_list != NULL);

	node *scope_ident = (node *)calloc(1, sizeof(node));
	scope_ident->_ntype = scope_start;

	scope_ident->_id = start_node->_id;

	*expr_list = scope_ident;


	// the first node is the declaration of the scope
	
	// return the node list of all the statements
	// dump info is fed already

	temp = *(expr_list + counter);
	temp->prev = *expr_list;
	// scope_start dont need to to_dot
	// as we have the emit_header function
	do
	{	

		temp->to_dot(temp);
		counter++;
		temp = *(expr_list + counter);
	}while(temp != NULL);
	// following the statement list
	// the to_dot funtion of the expr is called

	free(expr_list);
	free(scope_ident);
}

static void 
emit_header(char *scpe, int start_id)
{
	fprintf(stdout, "digraph AST {\n");
	fprintf(stdout, "  graph [fontname=\"Times New Roman\",fontsize=10];\n");
	fprintf(stdout, "  node  [fontname=\"Courier New\",fontsize=10];\n");
	fprintf(stdout, "  edge  [fontname=\"Times New Roman\",fontsize=10];\n\n");
	
	fprintf(stdout, "  node%d [label=\"scope %s\",shape=box];\n", start_id, scpe);
	fflush(stdout);
}

void eval_statement(node *n, char *scope)
{
	// from this node we found our love
	// n contains name of the scope
	node **node_list;

	node_list = read_statement(n);
	// read up coming statements utill return
	dump_list(node_list, scope, n);

	// n is the start of the scope
	// to_dot is called only in dump_list
	// dump_node is used for debugging
	
	free(node_list);
}

size_t 
get_filesize(void)
{
	// only call this function before you move the ptr to file position
        fseek(fp, 0L, SEEK_END);
        size_t size =(size_t)ftell(fp);
        rewind(fp);
	return size;
}

void 
eval_file(char *name)
{
	node *n;// = (node *)calloc(1, sizeof(node)); 
	// n will be the root of ast
	// statement_list, in our example
	int target_id;
	// in this case we only have one target
	while(1)
	{
		n = eval_node();
	
		if(n == NULL)
		// EOF
			break;

		if(check_inner(n, "strg: main"))
		{
			target_id = n->_id;
		}
		//memset(n, 0, sizeof(node));
	}

	node *target_n = search_pool(target_id, pool, n_inpool);
	// eval a scope with name "main"
	assert(NULL != target_n);
	emit_header("main", target_n->_id);
	eval_statement(target_n, "main");


        if( fclose(fp) == EOF)
        {
                fprintf(stderr, "File close error:%s\n");
                fflush(stderr);
                exit(0);
        }

	fprintf(stdout, "} ");
	fflush(stdout);
}



