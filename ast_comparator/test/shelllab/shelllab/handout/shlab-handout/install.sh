#! /bin/bash
# install all the ast

FILENAME=2_tsh.c
DIR=../../../../../

i=63
while [ $i -ge 0 ]
do
	FILENAME="${i}_tsh.c"

	rm *.c.001t.tu
	cp cases/$FILENAME .
	# generate the .001t.tu file
	gcc -fdump-translation-unit -fno-builtin -ffreestanding $FILENAME

	# generate filename.scope.dot under ast_comparator/dot_obj/
	~/Documents/tiny-comparator/ast_comparator/src/tu_eater "$FILENAME.001t.tu" eval > ../../../../../dot_obj/"${FILENAME}.eval.dot"

	${DIR}/src/comparator ${DIR}/dot_obj/"$FILENAME.eval.dot" > ${DIR}/brk_tree/"$FILENAME.eval.tree"
	rm $FILENAME
	i=$[$i-1]
done

i=22
while [ $i -ge 0 ]
do
	FILENAME="prev_${i}_tsh.c"

	rm *.c.001t.tu
	cp cases/$FILENAME .
	# generate the .001t.tu file
	gcc -fdump-translation-unit -fno-builtin -ffreestanding $FILENAME

	# generate filename.scope.dot under ast_comparator/dot_obj/
	~/Documents/tiny-comparator/ast_comparator/src/tu_eater "$FILENAME.001t.tu" eval > ../../../../../dot_obj/"${FILENAME}.eval.dot"

	${DIR}/src/comparator ${DIR}/dot_obj/"$FILENAME.eval.dot" > ${DIR}/brk_tree/"$FILENAME.eval.tree"
	i=$[$i-1]
	rm $FILENAME
done

i=39
while [ $i -ge 0 ]
do
	if [ $i -eq 8 ]; then
		i=$[$i-1]

		continue
	fi
	if [ $i -eq 9 ]; then
		i=$[$i-1]

		continue
	fi
	if [ $i -eq 28 ]; then
		i=$[$i-1]

		continue
	fi
	if [ $i -eq 29 ]; then
		i=$[$i-1]

		continue
	fi
	if [ $i -eq 35 ]; then
		i=$[$i-1]

		continue
	fi
	if [ $i -eq 39 ]; then
		i=$[$i-1]
		continue
	fi

	FILENAME="reference_${i}_tsh.c"

	rm *.c.001t.tu
	cp cases/$FILENAME .
	# generate the .001t.tu file
	gcc -fdump-translation-unit -fno-builtin -ffreestanding $FILENAME

	# generate filename.scope.dot under ast_comparator/dot_obj/
	~/Documents/tiny-comparator/ast_comparator/src/tu_eater "$FILENAME.001t.tu" eval > ../../../../../dot_obj/"${FILENAME}.eval.dot"

	${DIR}/src/comparator ${DIR}/dot_obj/"$FILENAME.eval.dot" > ${DIR}/brk_tree/"$FILENAME.eval.tree"
	i=$[$i-1]
	rm $FILENAME
done

