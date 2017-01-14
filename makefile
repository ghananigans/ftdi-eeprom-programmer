CC = clang
CFLAGS = \
	-g \
	-Wall \
	-DDEBUG

OBJECT_DIR = obj
SOURCE_DIR = src
BUILD_DIR = build

.PHONY: default all clean

default: build
all: default

OBJECTS = \
	$(OBJECT_DIR)/main.o

BUILD = \
	$(BUILD_DIR)/ftdi_flasher

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(BUILD_DIR)/ftdi_flasher: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

build: $(BUILD)

clean:
	-rm -f $(OBJECT_DIR)/*.o
	-rm -f $(BUILD_DIR)/ftdi_flasher