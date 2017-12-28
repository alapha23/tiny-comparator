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

#define INNER_SIZE	512
#define NUM_NODE	128
// TODO
// Only check 128 nodes in the statement list

FILE *fp;

typedef struct _n
{
	NODE_TYPE _ntype;
	int	_id;
	void (* to_dot)(struct _n *);
	char *_inner;
} node;

/*
 * open argument file
 */
void open_file( char **argv);

/*
 * eat the tu and emit the bone
 */
void eval_file(char *name);

size_t get_filesize(void);

void eval_statement(node *n);
// recording function body from that node

/*
 * read a node from the fp
 * return NULL at EOF
 */
static node *eval_node(void);

static void eval_ntype(char *, node *);

static void stmt_to_dot(node *);
// it writes to the dot file

static void dump_node(node *);

static NODE_TYPE str2node(char *, node *);
// it also sets the to_dot according to node type

static void stub_to_dot(node *);
// stub function

static char peek(void);

static int check_inner(node *, char *);
// search for srcp: <filename>.c in the node


#endif

