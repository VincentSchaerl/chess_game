CXX = g++
CXX_LINUX_32 = g++ -m32
CXX_LINUX_64 = g++ -m64
CXX_WINDOWS_32 = i686-w64-mingw32-g++-posix
CXX_WINDOWS_64 = x86_64-w64-mingw32-g++-posix
CXXFLAGS = -I$(INCLUDE_DIR) -MMD -MP
CXXFLAGS_RELEASE = -I$(INCLUDE_DIR) -static

INCLUDE_DIR = inc
OBJ_DIR = out/obj
OBJ_DIR_DEBUG = out/obj_debug
RELEASE_DIR = out/release

SRC = src/main.cpp src/board.cpp src/conversions.cpp src/input_output.cpp src/move.cpp src/piece.cpp src/play.cpp
OBJ = $(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
OBJ_DEBUG = $(patsubst src/%.cpp, $(OBJ_DIR_DEBUG)/%.o, $(SRC))
DEPS = $(OBJ:.o=.d)
DEPS_DEBUG = $(OBJ_DEBUG:.o=.d)

PROGRAM_NAME = chess_game
TARGET = out/$(PROGRAM_NAME)
DEBUG_TARGET = out/$(PROGRAM_NAME)_debug
LINUX_32 = $(RELEASE_DIR)/$(PROGRAM_NAME)_linux_32_bit
LINUX_64 = $(RELEASE_DIR)/$(PROGRAM_NAME)_linux_64_bit
WINDOWS_32 = $(RELEASE_DIR)/$(PROGRAM_NAME)_windows_32_bit.exe
WINDOWS_64 = $(RELEASE_DIR)/$(PROGRAM_NAME)_windows_64_bit.exe

####################################################################################################

$(TARGET): $(OBJ)
	mkdir -p $(dir $@)
	$(CXX) $^ -o $@

-include $(DEPS)

$(OBJ_DIR)/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

####################################################################################################

valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all $(DEBUG_TARGET)

debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(OBJ_DEBUG)
	mkdir -p $(dir $@)
	$(CXX) $^ -o $@

-include $(DEPS_DEBUG)

$(OBJ_DIR_DEBUG)/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) -c $< -o $@ $(CXXFLAGS) -g

####################################################################################################

release:
	mkdir -p $(RELEASE_DIR)
	$(CXX_LINUX_32) $(SRC) -o $(LINUX_32) $(CXXFLAGS_RELEASE)
	$(CXX_LINUX_64) $(SRC) -o $(LINUX_64) $(CXXFLAGS_RELEASE)
	$(CXX_WINDOWS_32) $(SRC) -o $(WINDOWS_32) $(CXXFLAGS_RELEASE)
	$(CXX_WINDOWS_64) $(SRC) -o $(WINDOWS_64) $(CXXFLAGS_RELEASE)

####################################################################################################

all: $(TARGET) debug release

run: $(TARGET)
	$(TARGET)

####################################################################################################

clean:
	rm -rf out/*