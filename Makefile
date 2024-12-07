CFLAGS = -std=c++17 -O2 -Wall -Wextra
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

BASE = src/base/CommandPool.cpp src/base/Descriptor.cpp src/base/RenderPass.cpp src/base/SwapChain.cpp
BUFFER = src/buffer/VertexBuffer.cpp src/buffer/IndexBuffer.cpp src/buffer/UniformBuffer.cpp
LOADER = src/loader/FileLoader.h src/loader/ModelLoader.cpp src/loader/TextureLoader.cpp
WRAPPER = src/wrapper/LogicalDeviceWrapper.cpp src/wrapper/PhysicalDeviceWrapper.cpp src/wrapper/InstanceWrapper.cpp
OBJ = src/main.cpp src/VulkanContext.cpp src/Window.cpp src/Application.cpp src/Shader.cpp
SHADER = shaders/shader.vert.spv shaders/shader.frag.spv

Vulkan_Odyssey: $(OBJ) $(SHADER)
	g++ $(CFLAGS) -o Vulkan_Odyssey $(OBJ) $(LOADER) $(WRAPPER) $(LDFLAGS) $(BASE) $(BUFFER) library/tiny_obj_loader.h library/stb_image.h

%.vert.spv: %.vert
	glslangValidator -V -o $@ $<

%.frag.spv: %.frag
	glslangValidator -V -o $@ $<

.PHONY: test clean

test: Vulkan_Odyssey
	./Vulkan_Odyssey

clean:
	rm -f Vulkan_Odyssey
	rm -f shaders/*.spv
	rm -f log.txt
	clear

