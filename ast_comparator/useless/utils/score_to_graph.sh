#! /bin/bash
# score to graph

DIR=~/Documents/tiny-comparator/ast_comparator
SCOPE=eval
FILENAME=${DIR}/score/${SCOPE}_tsh.txt
# ca cr cp cc
OPTION=cc

# score the scope
# plagiarism percentage under $DIR/similarity

# rm ${DIR}/similarity/"Similarity_tsh.txt"

# SCOPR: eval 
#echo "SCOPE: ${SCOPE}" >> ${DIR}/similarity/"Similarity_tsh.txt"

# scored
#${DIR}/src/scorer $FILENAME ${SCOPE} >> ${DIR}/similarity/"Similarity_tsh.txt"


# dump similarity percentage to a .dot file
${DIR}/src/score_to_graph ${DIR}/similarity/"Similarity_tsh.txt" $SCOPE ${OPTION} > ${SCOPE}.${OPTION}.dot

dot -Tpdf ${SCOPE}.${OPTION}.dot -o ${DIR}/pdf_result/${SCOPE}.${OPTION}.pdf

# atril  ${DIR}/pdf_result/${SCOPE}.${OPTION}.pdf &

