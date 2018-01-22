FILE1=$1
FILE2=$2
# yell() { echo "$0: $*" >&2; }
# die() { yell "$*"; exit 111; }
# try() { "$@" || die "cannot $*"; }
#	
# Generate score for two brk trees

if [ -z "$FILE2" ]; then
        echo "Usage: bash ./compare.sh <file1> <file2>"
        echo "files should be under brk_tree"
        echo "example: bash ./compare.sh brk_tree/helloworld.c.tree brk_tree/helloworld2.c.tree"
        exit 1;
fi

# echo ${FILE1:9}  
java -jar apted.jar  -f $FILE1 $FILE2 >> score/score.txt
echo "${FILE1:9}   ${FILE2:9}" >> score/score.txt
