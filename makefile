CC = g++ -pipe
HEADER_PATH = source/header
SRC_PATH = source/src
BIN_PATH = bin
DEP_PATH = dep

_OBJ  = main.o

OBJ = $(patsubst %,$(BIN_PATH)/%,$(_OBJ))

CFLAGS = -std=c++0x -Wall -c -I $(HEADER_PATH)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_PATH)/$*.Td
GST_L = `pkg-config --libs gstreamer-1.0 gstreamer-rtsp-1.0 gstreamer-video-1.0 gstreamer-plugins-bad-1.0 gstreamer-plugins-good-1.0 gstreamer-plugins-base-1.0`
GST_C = `pkg-config --cflags gstreamer-1.0 gstreamer-rtsp-1.0 gstreamer-video-1.0 gstreamer-plugins-bad-1.0 gstreamer-plugins-good-1.0 gstreamer-plugins-base-1.0`
LIBS =
FINAL_EXEC = $(BIN_PATH)/MandrillCore

all: release

debug: CFLAGS += -ggdb3 -Og
debug: $(FINAL_EXEC)

release: CFLAGS += -march=native -O3
release: $(FINAL_EXEC)

fast: CFLAGS += -march=native -Ofast
fast: $(FINAL_EXEC)

$(FINAL_EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

$(BIN_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(BIN_PATH) $(DEP_PATH)
	$(CC) $(DEPFLAGS) -c -o $@ $< $(CFLAGS)
	@mv -f $(DEP_PATH)/$*.Td $(DEP_PATH)/$*.d

.PHONY: clean
clean:
	-@rm -r $(BIN_PATH)

$(DEP_PATH)/%.d: ;
.PRECIOUS: $(DEP_PATH)/%.d

-include $(patsubst %,$(DEP_PATH)/%.d,$(basename $(_OBJ)))

.PHONY: doc
doc:
	doxygen config/Doxyfile

# CC = g++ -pipe
# HEADER_PATH = source/header
# SRC_PATH = source/src
# BIN_PATH = bin
# DEP_PATH = dep

# _OBJ  = main.o FramesManager.o Analyser.o Printer.o Camera.o \
		ThreadPool.o Worker.o Processor.o Observer.o GreyProcessor.o Saver.o \
		MeanProcessor.o CodecsConfig.o jsoncpp.o MovementProcessor.o \
		SocketCommunicator.o \
		GstManager.o GstCore.o

# OBJ = $(patsubst %,$(BIN_PATH)/%,$(_OBJ))

# CFLAGS = -std=c++0x -Wall -c -I $(HEADER_PATH) $(GST_C) $(OPENCV_C)
# DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_PATH)/$*.Td
# OPENCV_L = `pkg-config --libs opencv`
# OPENCV_C = `pkg-config --cflags opencv`
# GST_L = `pkg-config --libs gstreamer-1.0 gstreamer-rtsp-1.0 gstreamer-video-1.0 gstreamer-plugins-bad-1.0 gstreamer-plugins-good-1.0 gstreamer-plugins-base-1.0`
# GST_C = `pkg-config --cflags gstreamer-1.0 gstreamer-rtsp-1.0 gstreamer-video-1.0 gstreamer-plugins-bad-1.0 gstreamer-plugins-good-1.0 gstreamer-plugins-base-1.0`
# LIBS = $(GST_L) $(OPENCV_L) -pthread
# FINAL_EXEC = $(BIN_PATH)/MandrillCore

# all: release

# debug: CFLAGS += -ggdb3 -Og
# debug: $(FINAL_EXEC)

# release: CFLAGS += -march=native -O3
# release: $(FINAL_EXEC)

# fast: CFLAGS += -march=native -Ofast
# fast: $(FINAL_EXEC)

# $(FINAL_EXEC): $(OBJ)
	# $(CC) -o $@ $^ $(LIBS)

# $(BIN_PATH)/%.o: $(SRC_PATH)/%.cpp
	# @mkdir -p $(BIN_PATH) $(DEP_PATH)
	# $(CC) $(DEPFLAGS) -c -o $@ $< $(CFLAGS)
	# @mv -f $(DEP_PATH)/$*.Td $(DEP_PATH)/$*.d

# .PHONY: clean
# clean:
	# -@rm -r $(BIN_PATH)

# $(DEP_PATH)/%.d: ;
# .PRECIOUS: $(DEP_PATH)/%.d

# -include $(patsubst %,$(DEP_PATH)/%.d,$(basename $(_OBJ)))

# .PHONY: doc
# doc:
	# doxygen config/Doxyfile
