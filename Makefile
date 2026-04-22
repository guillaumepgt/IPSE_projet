# Nom de l'exécutable
TARGET = go

# Compilateur utilisé
CC = gcc

# Options de compilation
CCFLAGS += -g -Wall -Wextra -I. -Iterminal

# Répertoire d'installation de la bibliothèque MRPiZ
LIB_MRPIZ = $(realpath $(wildcard ../lib_mrpiz*/))

# Options de compilation pour l'utilisation de la bibliothèque MRPiZ
export CCFLAGS += -DINTOX -DINTOX_ADDRESS=127.0.0.1 -DINTOX_PORT=12301
CCFLAGS += -I$(LIB_MRPIZ)/include/mrpiz/
LDFLAGS += -L$(LIB_MRPIZ)/lib/ -lintoxmrpiz -lintox -lm

# Fichiers objets
OBJS = main.o robot.o pilot.o autopilot.o

# Règle par défaut
all: $(TARGET)

# Construction de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Compilation de main.c
main.o: main.c autopilot.h
	$(CC) $(CCFLAGS) -c main.c -o main.o

# Compilation de robot.c
robot.o: robot.c robot.h
	$(CC) $(CCFLAGS) -c robot.c -o robot.o

# Compilation de pilot.c
pilot.o: pilot.c pilot.h robot.h move.h
	$(CC) $(CCFLAGS) -c pilot.c -o pilot.o

# Compilation de autopilot.c
autopilot.o: autopilot.c autopilot.h pilot.h robot.h move.h
	$(CC) $(CCFLAGS) -c autopilot.c -o autopilot.o

# Nettoyage
clean:
	rm -f $(TARGET) $(OBJS)