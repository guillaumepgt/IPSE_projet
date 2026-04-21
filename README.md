
Pour la compilation de main.c en main.o :
gcc -c -g -Wall main.c -DINTOX -DINTOX_ADDRESS=127.0.0.1 -DINTOX_PORT=12341 -I../lib_mrpiz-x86_64-v0.6.1/include/mrpiz/ -o main.o

Pour l'édition des liens et la génération de l'exécutable :
gcc main.o -L../lib_mrpiz-x86_64-v0.6.1/lib/ -lintoxmrpiz -lintox -o go
