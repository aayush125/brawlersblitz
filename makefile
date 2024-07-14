# all:
# 	g++ -I SDL2/include -L SDL2/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2

# Compiler
CC := g++
# Compiler flags
CFLAGS := -std=c++20 -Wall
# Include directories
INCLUDES := -Iinclude -Iinclude/headers
# Source files directory
SRC_DIR := src
# Object files directory
OBJ_DIR := obj
# Executable directory
BIN_DIR := bin
# SDL2 library directory
SDL_LIB_DIR := lib/SDL2
# SDL2_image library directory
SDL_IMG_LIB_DIR := lib/SDL2_image
# SDL2_ttf library directory
SDL_TTF_LIB_DIR := lib/SDL2_ttf
# SDL2 libraries
SDL_LIBS := -L$(SDL_LIB_DIR) -lmingw32 -lSDL2main -lSDL2
# SDL2_image libraries
SDL_IMG_LIBS := -L$(SDL_IMG_LIB_DIR) -lSDL2_image
# SDL2_ttf libraries
SDL_TTF_LIBS := -L$(SDL_TTF_LIB_DIR) -lSDL2_ttf

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
# Object files
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable name
EXEC := $(BIN_DIR)/game

# Default target
all: $(EXEC)

# Rule to link object files and generate executable
$(EXEC): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $(EXEC) $(SDL_LIBS) $(SDL_IMG_LIBS) $(SDL_TTF_LIBS)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create directories if they don't exist
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Include dependencies
-include $(OBJS:.o=.d)

# Rule to generate dependency files
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -MM -MT $(@:.d=.o) $< > $@

.PHONY: clean

# Clean rule
clean:
	@echo 'Cleaning up...'
ifeq ($(OS),Windows_NT)
	del /Q $(OBJ_DIR)\*.o $(OBJ_DIR)\*.d $(BIN_DIR)\game.exe
else
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(BIN_DIR)/game
endif