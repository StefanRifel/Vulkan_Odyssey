#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "../../library/tiny_obj_loader.h"

#include "../types/Vertex.h"

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

#include "../buffer/Buffer.h"

class ModelLoader {

public:
  static void loadModel(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer, std::string& modelPath);
};

#endif
