FILEARG=$1
FILEARGSIZE=${#FILEARG}
FILENAME="Unknown"
FILEDIR="Unknown"
SCOPE=$2
#STRING="dot"

if [ -z "$SCOPE" ]; then
        echo "Usage: bash ./install.sh <file name> <scope name>"
        echo "test files are supposed to be placed under ./test"
        echo "example: bash install.sh test/helloworld.c main"
        exit 1;
fi

if [ -z "$FILEARG" ]; then
        echo "Usage: bash ./install.sh <file name> <scope name>"
        echo "test files are supposed to be placed under ./test"
        echo "example: bash install.sh test/helloworld.c main"
        exit 1;
fi


i=$[$FILEARGSIZE-1]

while [ $i -ge 0 ]
do
	#echo ${FILEARG:$i:1}
	if [ ${FILEARG:$i:1} = '/' ]; then
		i=$[$i+1]
		FILENAME=${FILEARG:$i:$FILEARGSIZE}
		FILEDIR=${FILEARG:0:$i}

		# echo $FILEDIR
		# echo ${FILEARG:$i:$FILEARGSIZE}
		break
	fi
	i=$[$i-1]
done


cd src
bash gen_ast.sh $FILENAME $SCOPE $FILEDIR
gcc comparator.c -o comparator
./comparator ../dot_obj/"$FILENAME.dot"  > ../brk_tree/"$FILENAME.tree"
cd ..

