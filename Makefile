#
# makefile for the avalon compiler
#
# :author   Ntwali Bashige
# :year     2018-2019
# :email    ntwali.bashige@gmail.com
#

CC          := GCC
CFLAGS      := -std=c11 -g -Wall -Wextra -pedantic -DDEBUG
SRC_DIR     := src
INC         := -Isrc
BUILD_DIR   := build
BIN_DIR     := bin
TARGET      := $(BIN_DIR)/avalonc

SRC_EXT     := c
SOURCES     := $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))
OBJECTS     := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))


.PHONY: all
all: setup $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: setup
setup:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	@find . -exec touch {} \;
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
