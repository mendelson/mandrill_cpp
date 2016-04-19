CC = g++
HEADER_PATH = source/header
SRC_PATH = source/src
BIN_DIR = bin

_OBJ = main.o Core.o FramesManager.o Analyser.o Printer.o Camera.o ThreadPool.o
_OBJ += Worker.o Processor.o Observer.o GreyProcessor.o MeanCalculator.o
OBJ = $(patsubst %,$(BIN_DIR)/%,$(_OBJ))

DIRECTIVES = -std=c++0x -Wall -c -I $(HEADER_PATH)
OPENCV = `pkg-config --libs opencv` -O4
LIBS = $(OPENCV)
FINAL_EXEC = $(BIN_DIR)/MandrillCore


all: $(FINAL_EXEC)

$(FINAL_EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

$(BIN_DIR)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(DIRECTIVES)

debug: DIRECTIVES += -ggdb
debug: all

.PHONY: clean
clean:
	-@rm -r $(BIN_DIR)
