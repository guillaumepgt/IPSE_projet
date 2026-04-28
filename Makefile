# Nom de l'exécutable (unique !)
TARGET = go

CC = gcc
CCFLAGS += -g -Wall -Wextra -I. -Iterminal

LIB_MRPIZ = $(realpath $(wildcard ../lib_mrpiz*/) )

export CCFLAGS  += -DINTOX
CCFLAGS += -I$(LIB_MRPIZ)/include/mrpiz/
LDFLAGS += -L$(LIB_MRPIZ)/lib/ -lintoxmrpiz -lintox -lm

all: $(TARGET)

$(TARGET): main.o robot.o pilot.o copilot.o
	$(CC) main.o robot.o pilot.o copilot.o $(LDFLAGS)  -o $(TARGET)

main.o: main.c move.h pilot.h copilot.h
	$(CC) $(CCFLAGS) -c main.c -o main.o

robot.o: robot.c robot.h
	$(CC) $(CCFLAGS) -c robot.c -o robot.o

pilot.o: pilot.c pilot.h robot.h move.h
	$(CC) $(CCFLAGS) -c pilot.c -o pilot.o

copilot.o: copilot.c copilot.h move.h
	$(CC) $(CCFLAGS) -c copilot.c -o copilot.o

clean:
	rm -f $(TARGET) *.o