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

static void sub_stmt_to_dot(node *n)
{
	node **node_list;
	int num_stmt;

 	char *inner = n->_inner;
	int pos = strlen(inner)-1;
	while(pos)
	{
		if(*(inner+pos) == ':')
		{
			// the last node
			pos--;
			while(1)
			{
				if(*(inner+pos) != ' ')
					break;
				pos--;
			}
			while(1)
			{
				if(*(inner+pos) != ' ')
				{
					pos--;
				}else
					break;
			}
			break;
		}
		pos--;
	}
	pos++;
        sscanf(inner+pos, "%d ", &num_stmt);

	// num_stmt+2
	// prev node is included
	node_list = calloc(num_stmt+2, sizeof(node *));

	*node_list = n->prev;

	// we use prev node
	// as current node is stmt_list
	pos = 3;
	int nstmt = 1;
	while(pos != strlen(inner))
	{
		if(*(inner+pos) == '@')	
		{
			node *temp;
			int temp_id;
			sscanf(inner+pos, "@%d ", &temp_id);
			temp = search_pool(temp_id, pool, n_inpool);

			*(node_list+nstmt) = temp;
			// it is vital to connect them with prev ptr
			temp->prev = *(node_list+nstmt-1);
			nstmt++;
		}
		pos++;	
	}

	// now dump all the stmt
	// now pos stands for which node in the list we are dumping
	pos = 1;	
	while(pos != nstmt)
	{
		(*(node_list+pos))->to_dot(*(node_list+pos));
		pos++;
	}
}

static void addr_to_dot(node *n)
{
	node *ptr_type;
	node *type;
	int ptr_type_id, type_id;

	// check type of addr
	sscanf(n->_inner, " type: @%d ", &ptr_type_id);
	ptr_type = search_pool(ptr_type_id, pool, n_inpool);
	sscanf(ptr_type->_inner, " %*s %*s %*s %*d ptd : @%d", &type_id);
	type = search_pool(type_id, pool, n_inpool);

	if(type->_ntype == array_type)
	{
		// print if it is needed
		node *op;
		int op_id;
		sscanf(n->_inner, " type: @%*d op 0: @%d ", &op_id);
		op = search_pool(op_id, pool, n_inpool);
		op->prev = n->prev;
		op->to_dot(op);
	}
}

static void string_cst_to_dot(node *n)
{
	int size;
	int type_id, size_id;
	node *type;
	node *size_n;
	
	// obtain the size of the string
	sscanf(n->_inner, " type: @%d ", &type_id);
	type = search_pool(type_id, pool, n_inpool);

	sscanf(type->_inner, " size: @%d ", &size_id);
	size_n = search_pool(size_id, pool, n_inpool);

	sscanf(size_n->_inner, " type: @%*d %*s %d ", &size);

	char value[size];
	int pos = 0;
	char *inner = n->_inner + 12;
	// TODO
	//sscanf(n->_inner, " %*s %*s strg: %s", value);
	while(pos != strlen(inner))
	{
		if(*(inner+pos) == ':')
		{
			pos += 2;
			int counter = 0;
			while((*(inner+pos+counter) != 'l') && counter <= size)
			{
				value[counter] = *(inner+pos+counter);	
				counter++;
			}
			break;
		}
		pos++;
	}
	n->_dot_id = dot_shape(n->_id, value);
	dot_link(n->prev->_dot_id, n->_dot_id);
}

static void nop_to_dot(node *n)
{
	int op_id;
	node *op;

	sscanf(n->_inner, " type: @%*d op 0: @%d", &op_id);
	op = search_pool(op_id, pool, n_inpool);
	op->prev = n->prev;
	op->to_dot(op);
}

static void eq_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "==");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}


static void ne_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "!=");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}


static void gt_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, ">");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}

static void ge_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, ">=");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}

static void lt_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "<");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}

static void le_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "<=");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}


static void cond_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "if");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}
static void plus_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "plus_expr");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}

static void call_to_dot(node *n)
{
	node *fn;
	node *addr;
	node *name;
	node **argu;
	char label[64] = "call ";
	char func_name[48];
	int fn_id, addr_id, name_id;
	int num_arg;

/*
 * format of call_expr
 * type: @3	fn  : @xxxx	0   : @xxxx	1  : @xxxx	....
 * */

	// first obtain the name of the function
	sscanf(n->_inner, " type: %*s fn  : @%d ", &addr_id);
	addr = search_pool(addr_id, pool, n_inpool);
	sscanf(addr->_inner, " type: %*s op 0: @%d", &fn_id);
	fn = search_pool(fn_id, pool, n_inpool);
	sscanf(fn->_inner, " name: @%d ", &name_id);
	name = search_pool(name_id, pool, n_inpool);
	sscanf(name->_inner, " strg: %s ", func_name);

	strcat(label, func_name);

	n->_dot_id = dot_shape(n->_id, label);

	// obtain the argument list of the function
	// first decide num if arguments
	char *inner = n->_inner;
	int pos = strlen(inner)-1;
	while(pos)
	{
		if(*(inner+pos) == ':')
		{
			// the last node
			pos--;
			while(1)
			{
				if(*(inner+pos) != ' ')
					break;
				pos--;
			}
			while(1)
			{
				if(*(inner+pos) != ' ')
				{
					pos--;
				}else
					break;
			}
			break;
		}
		pos--;
	}
	pos++;
	if((*(inner+pos) >=48) && (*(inner + pos)<=57))
	{
		node *temp; 
		int arg_id;

		// if there is any argument
		sscanf(inner+pos, "%d ", &num_arg);

		argu = calloc(num_arg+1, sizeof(node *));
		pos += 4;
		do
		{
			// obtain node id of arguments
			sscanf(inner+pos+3, "%d", &arg_id);
			do{	
				pos--;
			}while(*(inner+pos)!= ':');
	
			temp = search_pool(arg_id, pool, n_inpool); 
			*(argu + num_arg) = temp; 
			num_arg--;

//			DEBUF("type:%d", temp->_id);
			temp->prev = n;
			temp->to_dot(temp);
		}while(num_arg+1);

	}
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}

static void post_inc_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "post_inc");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);	
}

static void post_dec_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "post_dec");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);	
}

static void pre_inc_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "pre_inc");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);	
}

static void pre_dec_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "pre_dec");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);	
}

static void binary_to_dot(node *n, char*type)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, type);
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}

static void var_decl_to_dot(node *n)
{
	int name_i;
	node *name_n;
	char name_c[32];
	sscanf(n->_inner, " name: @%d ", &name_i);
	name_n = search_pool(name_i, pool, n_inpool);
	
	sscanf(name_n->_inner, " strg: %s ", name_c);

	n->_dot_id = dot_shape(n->_id, name_c);
	dot_link(n->prev->_dot_id, n->_dot_id);
}

static void indirect_ref_to_dot(node *n)
{
	// this function is called as the left hand of a modification
	// when left hand is referencing a ref
	int var_id;
	node *var;

	// indirect ref only have one op
	sscanf(n->_inner, " type: @%*d op 0: @%d", &var_id );
	var = search_pool(var_id, pool, n_inpool);

	n->_dot_id = dot_shape(n->_id, "*");
	var->prev = n;
	var->to_dot(var);
	dot_link(n->prev->_dot_id, n->_dot_id);
}

static void integer_cst_to_dot(node *n)
{
	int value;
	char *buffer = calloc(1, 6);
	sscanf(n->_inner, " %*s %*s int: %d", &value);
	sprintf(buffer, "%d", value);
	n->_dot_id = dot_shape(n->_id, buffer);
	dot_link(n->prev->_dot_id, n->_dot_id);
}

static void div_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "div");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}

static void mod_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "mod");
	n->_dot_id = t_dot_id;

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}

static void mult_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "mult");
	n->_dot_id = t_dot_id;
	//

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}

static void modify_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	assert(op1 != NULL);
	assert(op2 != NULL);
	int t_dot_id = dot_shape(n->_id, "modify");
	n->_dot_id = t_dot_id;
	//

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);


	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}

static void pointer_plus_to_dot(node *n)
{
	int id1, id2;
	sscanf(n->_inner, "%*s @%*d op 0: @%d op 1: @%d ", &id1, &id2);

	node *op1 = search_pool(id1, pool, n_inpool);
	node *op2 = search_pool(id2, pool, n_inpool);

	n->_dot_id = dot_shape(n->_id, "poiner_plus");

	assert(op1 != NULL);
	assert(op2 != NULL);

	op1->prev = n;
	op2->prev = n;
	op1->to_dot(op1);
	// op1 is a vardecl, it should be emiting its name
	op2->to_dot(op2);

	// connect with the previous node
	dot_link_dt(n->prev->_dot_id, n->_dot_id);
}

static void ret_to_dot(node *n)
{
	int expr_id;
	node *expr;

	n->_dot_id = dot_shape(n->_dot_id, "return");

	sscanf(n->_inner, " %*s %*s expr: @%d", &expr_id);
	expr = search_pool(expr_id, pool, n_inpool);

	expr->prev = n;
	expr->to_dot(expr);	

	dot_link_dt(n->prev->_dot_id, n->_dot_id);
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
		case 'g':
			if(*(node_type+1) == 't')
			{
				_t = gt_expr;
				n->to_dot = gt_to_dot;
			}else if(*(node_type+1) == 'e')
			{
				_t = ge_expr;
				n->to_dot = ge_to_dot;
			}
			break;
		case 'l':
			if(*(node_type+1) == 't')
			{
				_t = lt_expr;
				n->to_dot = lt_to_dot;
			}else if(*(node_type+1) == 'e')
			{
				_t = le_expr;
				n->to_dot = le_to_dot;
			}
			break;
		case 's':
			switch(*(node_type+2))
			{
				case 'a':
					_t = statement_list;
					n->to_dot = sub_stmt_to_dot;
					break;
				case 'r':
					_t = string_cst;
					n->to_dot = string_cst_to_dot;
					break;
				default:
					DEBUF("Unknown node type: %s", node_type);
			}
			break;
		case 'i':
			switch(*(node_type+8))
			{
				case 'e':
	 				_t = identifier_node;
					break;
				case 'c':
					_t = integer_cst;
					n->to_dot = integer_cst_to_dot;
					break;
				case 't':
					_t = integer_type;
					break;
				case '_':
					_t = indirect_ref;
					n->to_dot = indirect_ref_to_dot;
					break;
				default:
					DEBUF("Unknown node type: %s", node_type);
			}
			break;
		case 'n':
			switch(*(node_type+1))
			{
				case 'e':
					_t = ne_expr;
					n->to_dot = ne_to_dot;
					break;
				case 'o':
					_t = nop_expr;
					n->to_dot = nop_to_dot;
					break;
				default:
					DEBUF("Unknown node type: %s", node_type);
			}
			break;
		case 'e':
			_t = eq_expr;
			n->to_dot = eq_to_dot;
			break;
		case 'p':
			switch(*(node_type + 4))
			{
				case 't':
					if(*(node_type+8) == 'p')
					{
						_t = pointer_plus_expr;
						n->to_dot = pointer_plus_to_dot;
					}
					else
					{
						_t = pointer_type;
					}
					break;
				case 'n':
					_t = preincrement_expr;
					n->to_dot = pre_inc_to_dot;
					break;
				case 'e':
					_t = predecrement_expr;
					n->to_dot = pre_dec_to_dot;
					break;
				case 'i':
					_t = postincrement_expr;
					n->to_dot = post_inc_to_dot;
					break;
				case 'd':
					_t = postdecrement_expr;
					n->to_dot = post_dec_to_dot;
					break;
				case '_':
					_t = plus_expr;
					n->to_dot = plus_to_dot;
					break;
				default:
					fprintf(stderr, "Unknown node type:%s\n", node_type);
					fflush(stderr);
					exit(0);
			}
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
			if(*(node_type+1) == 'o')
			{
				_t = modify_expr;
				n->to_dot = modify_to_dot;
			}
			else if(*(node_type+1) == 'u')
			{
				_t = mult_expr;
				n->to_dot = mult_to_dot;
			}
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
			{
				switch(*(node_type+6))
				{
					case 'd':
						_t = trunc_div_expr;
						n->to_dot = div_to_dot;
						break;
					case 'm':
						_t = trunc_mod_expr;
						n->to_dot = mod_to_dot;
						break;
					case 'i':
						_t = tree_list;
						break;
				}
			}
			else if(*(node_type+9) == 'y')
				_t = type_decl;
			break;
		case 'c':
			switch(*(node_type+2))
			{
				case 'l':
					_t = call_expr;
					n->to_dot = call_to_dot;
					break;
				case 'm':
					_t = complex_type;
					break;
				case 'n':
					_t = cond_expr;
					n->to_dot = cond_to_dot;
					break;
				default:
					DEBUF("Unknown node type: %s", node_type);
					exit(0);
			}
			break;
		case 'a':
			switch(*(node_type + 1))
			{
				case 'r':
					_t = array_type;
					break;
				case 'd':
					_t = addr_expr;
					n->to_dot = addr_to_dot;
					break;
				default:
					DEBUF("Unknown node type: %s", node_type);
			}
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
	// connect expr list w
	temp->prev = *expr_list;
	temp->prev->_dot_id = 1;
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
	
	fprintf(stdout, "  node%d [label=\"scope %s\",shape=box];\n", acquire_id(), scpe);
	inc_id();
	fflush(stdout);
}

static void eval_statement(node *n, char *scope)
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
	// identifier node of main function, in our example
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



