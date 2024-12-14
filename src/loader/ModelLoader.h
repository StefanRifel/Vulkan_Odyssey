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
  tinyobj::attrib_t attrib;                         // container holds all of the positions, normals and texture coordinates in its attrib.vertices, attrib.normals and attrib.texcoords
  std::vector<tinyobj::shape_t> shapes;             // container contains all of the separate objects and their faces
  std::vector<tinyobj::material_t> materials;       // materials used in the OBJ file will be stored. Each material contains properties like color, texture, and other material attributes
  std::string warn, err;

public:
  void loadModel(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::string& modelPath);
};

#endif
