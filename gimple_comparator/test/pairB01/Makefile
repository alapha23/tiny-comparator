CFLAGS=-fdump-tree-all -fno-builtin -ffreestanding 

SRC=helloworld.c
OUT=helloworld.no_header.c

all: rm_header
	gcc $(CFLAGS) $(OUT)

rm_header:
	./rm_header $(SRC)

# fno-builtin: Don't recognize built-in functions that do not begin with __builtin_ as prefix.
#
# ffreestanding: Assert that compilation targets a freestanding environment.  This
#	 implies -fno-builtin.  A freestanding environment is one in which
# 	 the standard library may not exist, and program startup may not
#	 necessarily be at "main".  The most obvious example is an OS
#	 kernel.  This is equivalent to -fno-hosted.
#
# fdump-translation-unit-options (C++ only)
# 	 Dump a representation of the tree structure for the entire
#	 translation unit to a file. 


