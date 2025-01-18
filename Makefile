# Name der executable
PROGRAM_NAME = Vulkan_Odyssey

# Compiler
CXX = g++
CFLAGS = -std=c++17 -O2 -Wall -Wextra
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Verzeichnisse
SRC = src
SHADER = shaders
BIN = bin

# Target Namen
TARGET = $(PROGRAM_NAME)

# Alle Quell- und Objektdateien
SRCS = $(shell find $(SRC) -name '*.cpp')
OBJS = $(patsubst $(SRC)/%.cpp,$(BIN)/%.o,$(SRCS))

# Shader-Dateien
SHADER_SRC = $(wildcard $(SHADER)/*.vert) $(wildcard $(SHADER)/*.frag)
SHADER_SPV = $(SHADER_SRC:.vert=.vert.spv) $(SHADER_SRC:.frag=.frag.spv)

# Target
all: $(TARGET)

# Link main program
$(TARGET): $(OBJS) $(SHADER_SPV)
	$(CXX) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Regel für die Erstellung von Objektdateien
$(BIN)/%.o: $(SRC)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

# Shader-Kompilierung
%.vert.spv: %.vert
	mkdir -p $(dir $@)
	glslangValidator -V -o $@ $<

%.frag.spv: %.frag
	mkdir -p $(dir $@)
	glslangValidator -V -o $@ $<

clean:
	rm -rf $(TARGET) $(BIN) $(SHADER)/*.spv
	clear


