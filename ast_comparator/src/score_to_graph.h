#ifndef SCORE_TO_GRAPH
#define SCORE_TO_GRAPH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define N_REF	512	// Number of references to be compared to
#define	N_SCOPE	32
#define N_CUR	128

#define DEBUG(a) fprintf(stderr, #a"\n"); \
	fflush(stderr)
#define DEBUF(a, b)	fprintf(stderr, a"\n", (b)); \
	fflush(stderr)
#define readline(line, size, fp) if(fgets((line), 128, fp)==NULL) \
	{DEBUG(Unexpected reached EOF); exit(0);}

typedef struct _p
{
	char *myname;
	char *hername;
	char scope[64];	// the relationship exist within a scope
	int target_id;	// my target_id of my parent node
	int hertype;	// 0 for cur, 1 for prev, 2 for ref
	float match;	
} plagiarism;

typedef struct _n
{
	int id;			// id in pool
	char filename[64];
	int n_target;		// how many target node has
	plagiarism target[N_REF];
} node;				// node stands of the left hand side of comparison

// fileptr for Similarity_tsh.txt
FILE *fp = NULL;
// N_SCOPE scopes with length of 64
char scopes[N_SCOPE][64] = {'\0'};

int n_inpool = 0;
node **pool = NULL;	// pool contains all relationships, namely nodes

// Open file
void open_file(char **argv);

// get file size
size_t get_filesize(void);

// put all data inside the pool
void fill_pool(void);


// emit_header
void emit_header(char *scope, char *whovswho);

// Relationship between current & ref > cur_ref_tsh.dot
// Relationship between current & prev
// Relationship between current & cur
// Relationship between current & ALL



#endif
