# Nom de l'exécutable
TARGET = go

# Compilateur utilisé
CC = gcc

# Options de compilation
CCFLAGS += -g -Wall -Wextra -I.


# Répertoire d'installation de la bibliothèque MRPiZ
# pour la compilation à destination de la cible MRPiZ (à adapter) :
LIB_MRPIZ = $(realpath $(wildcard ../lib_mrpiz*/) )

# Options de compilation pour l'utilisation de la bibliothèque MRPiZ
export CCFLAGS  += -DINTOX -DINTOX_ADDRESS=127.0.0.1 -DINTOX_PORT=12341
CCFLAGS += -I$(LIB_MRPIZ)/include/mrpiz/
LDFLAGS += -L$(LIB_MRPIZ)/lib/ -lintoxmrpiz -lintox




# Règle par défaut
all: $(TARGET)

# Construction de l'exécutable à partir des fichiers objets
$(TARGET): main.o remote.o robot.o terminal/terminal.o
	$(CC) main.o remote.o robot.o terminal/terminal.o $(LDFLAGS)  -o $(TARGET)

# Compilation de main.c en main.o
main.o: main.c remote.h robot.h
	$(CC) $(CCFLAGS) -c main.c -o main.o

# Compilation de remote.c en remote.o
remote.o: remote.c remote.h terminal/terminal.h
	$(CC) $(CCFLAGS) -c remote.c -o remote.o

# Compilation de robot.c en robot.o
robot.o: robot.c robot.h remote.h
	$(CC) $(CCFLAGS) -c robot.c -o robot.o

# Compilation de terminal.c en terminal.o
terminal.o: terminal/terminal.c terminal/terminal.h 
	$(CC) $(CCFLAGS) -c terminal/terminal.c -o terminal/terminal.o

# Nettoyage des fichiers générés
clean:
	rm -f $(TARGET) main.o remote.o robot.o
