#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "../../library/tiny_obj_loader.h"

#include "../types/Vertex.h"

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

class ModelLoader {

private:
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;
  const std::string MODEL_PATH = "assets/models/viking_room.obj";

public:
  void loadModel(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
};

#endif
