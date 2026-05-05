# Nom de l'exécutable
TARGET = go

# Compilateur utilisé
CC = gcc

# Options de compilation
CCFLAGS += -g -Wall -Wextra -I. -Iterminal


# Répertoire d'installation de la bibliothèque MRPiZ
# pour la compilation à destination de la cible MRPiZ (à adapter) :
LIB_MRPIZ = $(realpath $(wildcard ../lib_mrpiz*/) )
LIB_LUMPIZ= $(realpath $(wildcard ../lib_lumpiz*/) )

# Options de compilation pour l'utilisation de la bibliothèque MRPiZ
export CCFLAGS  += -DINTOX -DINTOX_ADDRESS=127.0.0.1 -DINTOX_PORT=12301
CCFLAGS += -I$(LIB_MRPIZ)/include/mrpiz/ -I$(LIB_LUMPIZ)/include/lumpiz/


LDFLAGS += -L$(LIB_MRPIZ)/lib/ -lintoxmrpiz -lintox -L$(LIB_LUMPIZ)/lib/ -lintoxlumpiz  -lintox -pthread -lm





# Règle par défaut
all: $(TARGET)

# Construction de l'exécutable à partir des fichiers objets
$(TARGET): main_copy.o robot_copy.o pilot.o  surprises.o copilot.o
	$(CC) main_copy.o robot_copy.o pilot.o surprises.o copilot.o $(LDFLAGS)  -o $(TARGET) 

# Compilation de main.c en main.o
main_copy.o: main_copy.c pilot.h move.h surprises.h copilot.h 
	$(CC) $(CCFLAGS) -c main_copy.c -o main_copy.o

# Compilation de robot.c en robot.o
robot_copy.o: robot_copy.c robot_copy.h
	$(CC) $(CCFLAGS) -c robot_copy.c -o robot_copy.o

pilot.o: pilot.c pilot.h robot_copy.h move.h
	$(CC) $(CCFLAGS) -c pilot.c -o pilot.o

# Compilation de copilot.c en copilot.o
copilot.o: copilot.c copilot.h move.h
	$(CC) $(CCFLAGS) -c copilot.c -o copilot.o

surprises.o: surprises.c surprises.h
	$(CC) $(CCFLAGS) -c surprises.c -o surprises.o

# Nettoyage des fichiers générés
clean:
	rm -f $(TARGET) main_copy.o robot_copy.o pilot.o surprises.o copilot.o
