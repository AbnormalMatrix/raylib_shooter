CC = zig cc

# Shared C flags for compiling Raylib from source
CFLAGS = -Wall -Wextra -std=c99 \
         -Isrc \
         -Isrc/external/glfw/include \
         -Isrc/external/glfw/deps \
         -DPLATFORM_DESKTOP

# OS Detection and platform configuration
ifeq ($(OS),Windows_NT)
    TARGET_EXT = .exe
    LIBS = -lopengl32 -lgdi32 -lwinmm
    MKDIR = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    RM = if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
    RUN = .\$(TARGET)
else
    # Linux / macOS
    TARGET_EXT =
    CFLAGS += -D_DEFAULT_SOURCE
    LIBS = -lGL -lm -lpthread -ldl -lrt -lX11
    MKDIR = mkdir -p $(BUILD_DIR)
    RM = rm -rf $(BUILD_DIR)
    RUN = ./$(TARGET)
endif

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/game$(TARGET_EXT)

# Gather all .c files (your code + Raylib source files in src/)
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(OBJS) -o $@ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	$(MKDIR)

clean:
	$(RM)

run: all
	$(RUN)

.PHONY: all clean run