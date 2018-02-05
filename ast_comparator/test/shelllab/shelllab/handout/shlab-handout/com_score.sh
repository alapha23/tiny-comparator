#! /bin/bash
# generate compare score

FILENAME1=0_tsh.c
FILENAME2=reference_0_tsh.c.eval.tree
SCOPE=eval
DIR=../../../../../

# echo "${SCOPE}.tree"

# current students vs. reference

echo " " >> $DIR/score/"${SCOPE}_tsh.txt"
echo " " >> $DIR/score/"${SCOPE}_tsh.txt"
echo "Current students vs. reference " >> $DIR/score/"${SCOPE}_tsh.txt"
echo " " >> $DIR/score/"${SCOPE}_tsh.txt"
	
i=63
j=39
while [ $i -ge 0 ]
do
	FILENAME1="${i}_tsh.c.${SCOPE}.tree"
	FILENAME2="reference_${j}_tsh.c.${SCOPE}.tree"

	echo " " >> $DIR/score/"${SCOPE}_tsh.txt" 
	echo " " >> $DIR/score/"${SCOPE}_tsh.txt"
	echo "${FILENAME1} vs. ALL REFERENCES" >> $DIR/score/"${SCOPE}_tsh.txt" 
	echo "${FILENAME1} vs. ALL REFERENCES" 

	while [ $j -ge 0 ]
	do
		if [ $j -eq 8 ]; then
			j=$[$j-1]
			continue
		fi
		if [ $j -eq 9 ]; then
			j=$[$j-1]
			continue
		fi
		if [ $j -eq 28 ]; then
			j=$[$j-1]
			continue
		fi
		if [ $j -eq 29 ]; then
			j=$[$j-1]
			continue
		fi
		if [ $j -eq 35 ]; then
			j=$[$j-1]
			continue
		fi
		if [ $j -eq 39 ]; then
			j=$[$j-1]
			continue
		fi
		FILENAME2="reference_${j}_tsh.c.${SCOPE}.tree"
		java -jar $DIR/apted.jar -f $DIR/brk_tree/$FILENAME1 $DIR/brk_tree/$FILENAME2 >> $DIR/score/"${SCOPE}_tsh.txt"
		echo "$FILENAME1 vs. ref: $FILENAME2" >> $DIR/score/"${SCOPE}_tsh.txt"		
		j=$[$j-1]
	done

	i=$[$i-1]
	j=39
done


# current students vs. prev
#

echo " " >> $DIR/score/"${SCOPE}_tsh.txt"
echo " " >> $DIR/score/"${SCOPE}_tsh.txt"
echo "Current students vs. prev " >> $DIR/score/"${SCOPE}_tsh.txt"
echo " " >> $DIR/score/"${SCOPE}_tsh.txt"
	
i=63
j=22
while [ $i -ge 0 ]
do
	FILENAME1="${i}_tsh.c.${SCOPE}.tree"
	FILENAME2="prev_${j}_tsh.c.${SCOPE}.tree"

	echo " " >> $DIR/score/"${SCOPE}_tsh.txt" 
	echo " " >> $DIR/score/"${SCOPE}_tsh.txt"
	echo "${FILENAME1} vs. ALL PREV" >> $DIR/score/"${SCOPE}_tsh.txt" 
	echo "${FILENAME1} vs. ALL PREV" 

	while [ $j -ge 0 ]
	do
		FILENAME2="prev_${j}_tsh.c.${SCOPE}.tree"
		java -jar $DIR/apted.jar -f $DIR/brk_tree/$FILENAME1 $DIR/brk_tree/$FILENAME2 >> $DIR/score/"${SCOPE}_tsh.txt"
		echo "$FILENAME1 vs. ref: $FILENAME2" >> $DIR/score/"${SCOPE}_tsh.txt"		
		j=$[$j-1]
	done

	i=$[$i-1]
	j=22
done

# current students vs. current students
i=63
j=63

echo " " >> $DIR/score/"${SCOPE}_tsh.txt"
echo " " >> $DIR/score/"${SCOPE}_tsh.txt"
echo "Current students vs. Current students " >> $DIR/score/"${SCOPE}_tsh.txt"
echo " " >> $DIR/score/"${SCOPE}_tsh.txt"
	
while [ $i -ge 0 ]
do
	FILENAME1="${i}_tsh.c.${SCOPE}.tree"
	FILENAME2="${j}_tsh.c.${SCOPE}.tree"

	echo " " >> $DIR/score/"${SCOPE}_tsh.txt" 
	echo " " >> $DIR/score/"${SCOPE}_tsh.txt"
	echo "${FILENAME1} vs. ALL CURRENT" >> $DIR/score/"${SCOPE}_tsh.txt" 
	echo "${FILENAME1} vs. ALL CURRENT" 

	while [ $j -ge 0 ]
	do
		if [ $j -eq $i ]; then
			j=$[$j-1]
			continue
		fi
		FILENAME2="${j}_tsh.c.${SCOPE}.tree"
		java -jar $DIR/apted.jar -f $DIR/brk_tree/$FILENAME1 $DIR/brk_tree/$FILENAME2 >> $DIR/score/"${SCOPE}_tsh.txt"
		echo "$FILENAME1 vs. ref: $FILENAME2" >> $DIR/score/"${SCOPE}_tsh.txt"		
		j=$[$j-1]
	done

	i=$[$i-1]
	j=63
done


