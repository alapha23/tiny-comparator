### What are considered to be SCP or CC
### Plagiarism-obscuring "Attacks"

* Fuzzy variable names--ambiguities in program semantics
* variable whitespace
* nonlinear sequencing of code
* present difficulties in textual similarity analysis unique
to program source code.

* comment alteration
* stub statement--> compare average of two tree-to-empty with tree-to-tree
* whilespace padding

```clike=
// code 1
	int main(){printf("Aloha");}
// code 2
	int 
	main()
	{printf("Aloha");}
```
* identifier renaming
* code reordering: order of decl won't affect

* algebraic expressions --> a*b <==> b*a
there is no difference between * and /
so a = b -> a = b*a/a*a/a*a/a will crack the comparator


do while <--> while

cross difference scopes

### Objective 

* textual plagiarism detection --> not in our scope
* source code plagiarism detection
* code clone detection --> vimdiff
	* AST & Program Dependence Graphs(cfg) are used

depends on determining the similarity of **arbitrary code segment**


### Methods

* n-grams
* latent semantic analysis
* winnowing -- MOSS stanford

#### edit distance: 
minimum number of applications from a defined set of operations necessary to transform one instance of a structure to another

* Levenshtein String Edit Distance[8]
* RTED
* Graph Edit Distance
* Document Fingerprinting: comparing hash values


### Note

* clone code detection & source code plagiarism
* MOSS should use a breadth of similarity measures

processing .dot file
RTED tree edit distance algorithm
C++ Boost lib / perl lab: a robust regular expressions library is required for text preprocessing and manipulation

### Problem

* Lack of suitable testing corpus of plagiarized C programs

* First generate GIMPLE, then generate ast & do comparison

* Difference Between many trees rather than one tree

* Make tests 

* try other algorithms

* support for c++

* Multiscope tree compare


---

today

read scope

generate dot tree of all scope
generate brk tree of all scope

read student list
compare students with references

generate score of each scope

weight overall score & similarity




