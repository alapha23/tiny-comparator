#! /bin/bash
# install all the ast

FILENAME=2_tsh.c
DIR=../../../../../

i=39
while [ $i -ge 30 ]
do
	FILENAME="reference_${i}_tsh.c"

	rm *.c.001t.tu
	cp cases/$FILENAME .
	# generate the .001t.tu file
	gcc -fdump-translation-unit -fno-builtin -ffreestanding $FILENAME

	# generate filename.scope.dot under ast_comparator/dot_obj/
	~/Documents/tiny-comparator/ast_comparator/src/tu_eater "$FILENAME.001t.tu" eval > ../../../../../dot_obj/"${FILENAME}.eval.dot"

	${DIR}/src/comparator ${DIR}/dot_obj/"$FILENAME.eval.dot" > ${DIR}/brk_tree/"$FILENAME.eval.tree"
	i=$[$i-1]
done

