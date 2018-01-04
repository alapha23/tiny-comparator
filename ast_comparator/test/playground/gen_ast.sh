FILENAME=$1
rm rm_header *.*.*
rm tu_eater

gcc -o tu_eater tu_eater.c tu_eater.h
gcc -o rm_header rm_header.c
./rm_header $FILENAME > temp.c
# helloworld.c -> helloworld.rm_header.c
gcc -fdump-translation-unit -fno-builtin -ffreestanding -c  temp.c
./tu_eater temp.c.001t.tu > ast.dot
#dot -Tpdf -o ast.pdf ast.dot 

rm temp.c *.o
unset FILENAME
