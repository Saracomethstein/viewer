#ifndef CPP4_3DVIEWER_V_2_0_1_SRC_BACKEND_MODEL_H_
#define CPP4_3DVIEWER_V_2_0_1_SRC_BACKEND_MODEL_H_

#include <string>
#include <vector>

#include "obj_parser.h"

namespace s21 {

class ViewerModel {
 public:
  bool LoadFromFile(const std::string& filename);
  void Move(int delta, char axis);
  void Rotate(double angle, char axis);
  void Scale(int scale);

  const std::vector<double>& getVertices() const { return vertices_; }
  const std::vector<int>& getFaces() const { return faces_; }
  const std::string& getFilename() const { return filename_; }
  size_t getVertexCount() const { return vertices_.size() / 3; }
  size_t getFaceCount() const { return faces_.size(); }

 private:
  std::vector<double> vertices_;
  std::vector<int> faces_;
  std::string filename_;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V_2_0_1_SRC_BACKEND_MODEL_H_
