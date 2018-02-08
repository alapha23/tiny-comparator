#! /bin/bash
# score to graph

DIR=~/Documents/tiny-comparator/ast_comparator
SCOPE=sigtstp_handler
FILENAME=${DIR}/score/${SCOPE}_tsh.txt

# score the scope
# plagiarism percentage under $DIR/similarity

# rm ${DIR}/similarity/"Similarity_tsh.txt"

# SCOPR: eval 
echo "SCOPE: ${SCOPE}" >> ${DIR}/similarity/"Similarity_tsh.txt"

# scored
${DIR}/src/scorer $FILENAME ${SCOPE} >> ${DIR}/similarity/"Similarity_tsh.txt"

# dump similarity percentage to a .dot file



