CC = zig cc
CFLAGS = -Wall -Wextra -std=c99
LIBS = $(shell pkg-config --libs raylib 2>/dev/null || echo "-lraylib -lGL -lm -lpthread -ldl -lrt -lX11")
INCLUDES = $(shell pkg-config --cflags raylib 2>/dev/null)

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/game

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(OBJS) -o $@ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(TARGET)

.PHONY: all clean run