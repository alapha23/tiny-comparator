FILENAME=$1
SCOPE=$2
STRING="dot"

if [ -z "$SCOPE" ]; then
        echo "Usage: bash ./install.sh <file name> <scope name>"
        echo "test files are supposed to be placed under ./test"
        echo "example: bash install.sh test/helloworld.c main"
        exit 1;
fi

cd src
bash gen_ast.sh $FILENAME $SCOPE
gcc comparator.c -o comparator
./comparator ../obj/"${FILENAME:5}.$STRING"  > ../result/"${FILENAME:5}.tree"
cd ..


