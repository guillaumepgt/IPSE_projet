# Nom de l'exécutable
TARGET = go

# Compilateur utilisé
CC = gcc

# Options de compilation
CCFLAGS += -g -Wall -Wextra -I. -Iterminal

# Répertoire d'installation de la bibliothèque MRPiZ
LIB_MRPIZ = $(realpath $(wildcard ../lib_mrpiz*/))

# Options de compilation pour l'utilisation de la bibliothèque MRPiZ
export CCFLAGS  += -DINTOX
CCFLAGS += -I$(LIB_MRPIZ)/include/mrpiz/
LDFLAGS += -L$(LIB_MRPIZ)/lib/ -lintoxmrpiz -lintox -lm

# Règle par défaut
all: $(TARGET)

# Construction de l'exécutable à partir des fichiers objets
$(TARGET): main.o remote.o robot.o autopilot.o input_detector.o terminal/terminal.o
	$(CC) main.o remote.o robot.o input_detector.o terminal/terminal.o $(LDFLAGS) -o $(TARGET)

# Compilation de main.c en main.o
main.o: main.c input_detector.h autopilot.h
	$(CC) $(CCFLAGS) -c main.c -o main.o

# Compilation de input_detector.c en input_detector.o
input_detector.o: input_detector.c input_detector.h robot.h remote.h
	$(CC) $(CCFLAGS) -c input_detector.c -o input_detector.o

# Compilation de remote.c en remote.o
remote.o: remote.c remote.h terminal/terminal.h
	$(CC) $(CCFLAGS) -c remote.c -o remote.o

# Compilation de robot.c en robot.o
robot.o: robot.c robot.h remote.h
	$(CC) $(CCFLAGS) -c robot.c -o robot.o

# Compilation de terminal.c en terminal.o
terminal.o: terminal/terminal.c terminal/terminal.h 
	$(CC) $(CCFLAGS) -c terminal/terminal.c -o terminal/terminal.o

# Compilation de autopilot.c
autopilot.o: autopilot.c autopilot.h pilot.h robot.h move.h
	$(CC) $(CCFLAGS) -c autopilot.c -o autopilot.o

# Nettoyage des fichiers générés
clean:
	rm -f $(TARGET) main.o remote.o robot.o input_detector.o terminal/terminal.o
