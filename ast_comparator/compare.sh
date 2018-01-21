FILE1=$1
FILE2=$2
# yell() { echo "$0: $*" >&2; }
# die() { yell "$*"; exit 111; }
# try() { "$@" || die "cannot $*"; }

if [ -z "$FILE2" ]; then
        echo "Usage: bash ./compare.sh <file1> <file2>"
        echo "files should be under result"
        echo "example: bash ./compare.sh result/helloworld.c.tree helloworld2.c.tree"
        exit 1;
fi

java -jar apted.jar -f $FILE1 $FILE2
