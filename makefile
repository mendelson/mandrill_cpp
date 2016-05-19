CC = g++
HEADER_PATH = source/header
SRC_PATH = source/src
BIN_PATH = bin
DEP_PATH = dep

_OBJ  = main.o Core.o FramesManager.o Analyser.o Printer.o Camera.o ThreadPool.o
_OBJ += Worker.o Processor.o Observer.o GreyProcessor.o Saver.o MeanProcessor.o
_OBJ += CodecsConfig.o jsoncpp.o
OBJ = $(patsubst %,$(BIN_PATH)/%,$(_OBJ))

DIRECTIVES = -std=c++0x -Wall -c -I $(HEADER_PATH)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_PATH)/$*.Td
OPENCV = `pkg-config --libs opencv` -O4
LIBS = $(OPENCV)


FINAL_EXEC = $(BIN_PATH)/MandrillCore
all: $(FINAL_EXEC)
$(FINAL_EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

$(BIN_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(BIN_PATH) $(DEP_PATH)
	$(CC) $(DEPFLAGS) -c -o $@ $< $(DIRECTIVES)
	@mv -f $(DEP_PATH)/$*.Td $(DEP_PATH)/$*.d

debug: DIRECTIVES += -ggdb
debug: all

.PHONY: clean
clean:
	-@rm -r $(BIN_PATH)

$(DEP_PATH)/%.d: ;
.PRECIOUS: $(DEP_PATH)/%.d

-include $(patsubst %,$(DEP_PATH)/%.d,$(basename $(_OBJ)))
