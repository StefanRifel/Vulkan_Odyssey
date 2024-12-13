CXX = g++
CFLAGS = -std=c++17 -O2 -Wall -Wextra
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Verzeichnisse
SRC_DIR = src
SHADER_DIR = shaders
LIB_DIR = library
BUILD_DIR = build

# Quell- und Headerdateien
BASE_SRC = $(SRC_DIR)/base/CommandPool.cpp $(SRC_DIR)/base/Descriptor.cpp $(SRC_DIR)/base/RenderPass.cpp $(SRC_DIR)/base/SwapChain.cpp
BUFFER_SRC = $(SRC_DIR)/buffer/VertexBuffer.cpp $(SRC_DIR)/buffer/IndexBuffer.cpp $(SRC_DIR)/buffer/UniformBuffer.cpp
LOADER_SRC = $(SRC_DIR)/loader/ModelLoader.cpp $(SRC_DIR)/loader/TextureLoader.cpp
WRAPPER_SRC = $(SRC_DIR)/wrapper/LogicalDeviceWrapper.cpp $(SRC_DIR)/wrapper/PhysicalDeviceWrapper.cpp $(SRC_DIR)/wrapper/InstanceWrapper.cpp
OBJ_SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/Scene.cpp $(SRC_DIR)/Window.cpp $(SRC_DIR)/Application.cpp $(SRC_DIR)/Shader.cpp $(SRC_DIR)/Camera.cpp $(SRC_DIR)/InputManager.cpp

# Shader-Dateien
SHADER_SRC = $(SHADER_DIR)/shader.vert $(SHADER_DIR)/shader.frag
SHADER_SPV = $(SHADER_SRC:.vert=.vert.spv) $(SHADER_SRC:.frag=.frag.spv)

# Alle Quell- und Objektdateien
SRC_FILES = $(BASE_SRC) $(BUFFER_SRC) $(LOADER_SRC) $(WRAPPER_SRC) $(OBJ_SRC)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Ziel
TARGET = Vulkan_Odyssey

# Regeln
all: $(TARGET)

# Ziel für das finale Programm
$(TARGET): $(OBJ_FILES) $(SHADER_SPV)
	$(CXX) $(CFLAGS) -o $@ $(OBJ_FILES) $(LDFLAGS)

# Regel für die Erstellung von Objektdateien
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

# Shader-Kompilierung
%.vert.spv: %.vert
	glslangValidator -V -o $@ $<

%.frag.spv: %.frag
	glslangValidator -V -o $@ $<

# Zusätzliche Regeln
.PHONY: test clean

test: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(TARGET) $(BUILD_DIR) $(SHADER_DIR)/*.spv log.txt
	clear


