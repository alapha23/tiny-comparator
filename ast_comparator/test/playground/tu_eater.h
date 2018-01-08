#ifndef __EATER
#define __EATER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "def.h"

#define DEBUG(a) fprintf(stderr, #a"\n"); \
	fflush(stderr)
#define DEBUF(a, b) fprintf(stderr, a"\n", b); \
	fflush(stderr)
#define char2int(c) ((int)c - 48)
#define dot_shape(id, label) fprintf(stdout, "  node%d [label=\"%s\",shape=ellipse];\n", id, label); \
	fflush(stdout)
#define dot_link(id1, id2)  fprintf(stdout, "  node%d->node%d;\n", id1, id2); \
	fflush(stdout)
#define dot_link_dt(id1, id2)  fprintf(stdout, "  node%d -> node%d [style=dotted]\n", id1, id2); \
	fflush(stdout)
#define ADD2POOL(node, pool, n)  *(pool+n) = node; \
				     	n++
#define get_by_num(num, pool, n) ({ \
	    (*(pool+num)); \
	    })
#define search_pool(id, pool, n_inpool) ({ \
		int i, check;	\
	       	for(i=id-1; i < n_inpool; i++) \
		{if(id == (get_by_num(i, pool, n)->_id)) {check=1;break;}} \
		if(check) \
		get_by_num(i, pool, n); \
		else for(i=0; i < n_inpool; i++) \
		{if(id == (get_by_num(i, pool, n)->_id)) break;} \
		get_by_num(i, pool, n); \
		})
#define get_next(n, pool, n_inpool) (get_by_num(n->_id, pool, n_inpool))

#define INNER_SIZE	512
#define NUM_EXPR	1024
#define NUM_NODE	128
// Assume number of stmt within a stmt list is within 1024
// TODO
// Only check 128 nodes in the statement list


typedef struct _n
{
	NODE_TYPE _ntype;
	int	_id;
	void (* to_dot)(struct _n *);
	char *_inner;
	struct _n * prev;		// prev node is connected when generating dot files
	struct _n ** dep_node;		// a list of nodes which are its dependencies
} node;


FILE	*fp;
node	**pool;		// a pool containing the nodes
int	n_inpool=0;	// number of nodes in pool

/*
 * open argument file
 */
void open_file( char **argv);

/*
 * eat the tu and emit the bone
 */
void eval_file(char *name);

size_t get_filesize(void);

void eval_statement(node *n, char* scope);
// recording function body from that node

/*
 * read a node from the fp
 * return NULL at EOF
 */
static node *eval_node(void);
// every node would be added to the pool

static void eval_ntype(char *, node *);

static void dump_node(node *);

static NODE_TYPE str2node(char *, node *);
// it also sets the to_dot according to node type

static char peek(void);

static int check_inner(node *, char *);
// search for srcp: <filename>.c in the node

static node** read_statement(node *);
// read all the necessary nodes for a statement_list
// return the node list pointing to a list of node pointers

static void dump_list(node **, char*, node *);
// argu: a list of nodes, name of the scope

static node **parse_stmt(node **node_list);
// return list of expr nodes
// node_list includes statement_list to return_expr

static void emit_header(char *scpe, int start_id);

static void stmt_to_dot(node *);
// it writes to the dot file

static void stub_to_dot(node *);
// stub function

static void modify_to_dot(node *);

static void ret_to_dot(node *);

static void var_decl_to_dot(node *);

static void integer_cst_to_dot(node *);
#endif

