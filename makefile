CC = clang
CFLAGS = \
	-g \
	-Wall \
	-DDEBUG \
	-Ilibs

INCLUDE = \
	-L./libs/ftd2xx/bin/10.5-10.7 \
	-lftd2xx.1.2.2

OBJECT_DIR = obj
SOURCE_DIR = src
BUILD_DIR = build

.PHONY: default all clean

default: build
all: default

HEADERS = \
	$(SOURCE_DIR)/ftdi.h

OBJECTS = \
	$(OBJECT_DIR)/ftdi.o \
	$(OBJECT_DIR)/main.o

BUILD = \
	$(BUILD_DIR)/ftdi_flasher


$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(BUILD_DIR)/ftdi_flasher: $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJECTS) -o $@

build: $(BUILD)

clean:
	-rm -f $(OBJECT_DIR)/*.o
	-rm -f $(BUILD_DIR)/ftdi_flasher