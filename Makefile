TARGET = go

BUILD_DIR = build
SRC_DIR = src

CC = gcc

CCFLAGS += -g -Wall -Wextra -I. -I$(SRC_DIR) -I$(SRC_DIR)/terminal

LIB_MRPIZ = $(realpath $(wildcard ../lib_mrpiz*/))
LIB_LUMPIZ = $(realpath $(wildcard ../lib_lumpiz*/))

export CCFLAGS  += -DINTOX
CCFLAGS += -I$(LIB_MRPIZ)/include/mrpiz/ -I$(LIB_LUMPIZ)/include/lumpiz/
LDFLAGS += -L$(LIB_MRPIZ)/lib/ -lintoxmrpiz -lintox -L$(LIB_LUMPIZ)/lib/ -lintoxlumpiz -lintox -pthread -lm

OBJS = $(BUILD_DIR)/main.o \
       $(BUILD_DIR)/remote.o \
       $(BUILD_DIR)/robot.o \
       $(BUILD_DIR)/pilot.o \
       $(BUILD_DIR)/autopilot.o \
       $(BUILD_DIR)/input_detector.o \
       $(BUILD_DIR)/terminal.o \
       $(BUILD_DIR)/copilot.o \
       $(BUILD_DIR)/surprises.o

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/input_detector.o: $(SRC_DIR)/input_detector.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $(SRC_DIR)/input_detector.c -o $(BUILD_DIR)/input_detector.o

$(BUILD_DIR)/remote.o: $(SRC_DIR)/remote.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $(SRC_DIR)/remote.c -o $(BUILD_DIR)/remote.o

$(BUILD_DIR)/robot.o: $(SRC_DIR)/robot.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $(SRC_DIR)/robot.c -o $(BUILD_DIR)/robot.o

# Attention au sous-dossier terminal
$(BUILD_DIR)/terminal.o: $(SRC_DIR)/terminal/terminal.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $(SRC_DIR)/terminal/terminal.c -o $(BUILD_DIR)/terminal.o

$(BUILD_DIR)/autopilot.o: $(SRC_DIR)/autopilot.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $(SRC_DIR)/autopilot.c -o $(BUILD_DIR)/autopilot.o

$(BUILD_DIR)/copilot.o: $(SRC_DIR)/copilot.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $(SRC_DIR)/copilot.c -o $(BUILD_DIR)/copilot.o

$(BUILD_DIR)/pilot.o: $(SRC_DIR)/pilot.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $(SRC_DIR)/pilot.c -o $(BUILD_DIR)/pilot.o

$(BUILD_DIR)/surprises.o: $(SRC_DIR)/surprises.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $(SRC_DIR)/surprises.c -o $(BUILD_DIR)/surprises.o

clean:
	rm -rf $(BUILD_DIR) $(TARGET)