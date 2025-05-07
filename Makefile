# Ścieżki
SRC_DIR := .
BUILD_DIR := build
LIB_DIR := ../../Libraries-OpenGL

# Dodatkowe katalogi MSYS2 dla GLFW
MSYS_INCLUDE := /mingw64/include
MSYS_LIB := /mingw64/lib
MSYS_DLL := /mingw64/bin/glfw3.dll

# Ścieżki do nagłówków i bibliotek
INCLUDE_DIRS := -I$(LIB_DIR)/include -I$(SRC_DIR) -I$(MSYS_INCLUDE)
LIBRARY_DIRS := -L$(LIB_DIR)/lib -L$(MSYS_LIB)

# Pliki źródłowe
SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(filter %.cpp,$(SOURCES)))
OBJECTS += $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(filter %.c,$(SOURCES)))

# Wynikowy plik .exe
TARGET := $(BUILD_DIR)/main.exe

# Kompilator i flagi
CXX := g++
CXXFLAGS := -std=c++17 -Wall $(INCLUDE_DIRS)
LDFLAGS := $(LIBRARY_DIRS) -lglfw3 -lopengl32 -lgdi32

# Shader i tekstury
SHADERS := $(wildcard $(SRC_DIR)/*.vert) $(wildcard $(SRC_DIR)/*.frag)
TEXTURES := $(wildcard $(SRC_DIR)/Textures/*)

# Domyślny cel
all: $(TARGET) copy-resources copy-dll

# Linkowanie
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Kompilacja .cpp → .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Kompilacja .c → .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Kopiowanie shaderów i tekstur
copy-resources:
	@echo "Copying shaders and textures..."
	@cp -u $(SHADERS) $(BUILD_DIR) 2>/dev/null || echo "No shaders to copy."
	@if [ -d "$(SRC_DIR)/Textures" ]; then \
		mkdir -p $(BUILD_DIR)/Textures; \
		cp -ru $(SRC_DIR)/Textures/* $(BUILD_DIR)/Textures; \
	else \
		echo "No Textures folder to copy."; \
	fi


# Kopiowanie glfw3.dll
copy-dll:
	@cp $(MSYS_DLL) $(BUILD_DIR)/

# Czysto
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean copy-resources copy-dll
