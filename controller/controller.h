#ifndef CPP4_3DVIEWER_V_2_0_1_SRC_CONTROLLER_CONTROLLER_H_
#define CPP4_3DVIEWER_V_2_0_1_SRC_CONTROLLER_CONTROLLER_H_

#include "../backend/model.h"

namespace s21 {

class Controller {
 public:
  Controller() = default;

  bool LoadFromFile(const std::string& filename) {
    return model_.LoadFromFile(filename);
  }

  void Move(int delta, char axis) { model_.Move(delta, axis); }

  void Rotate(double angle, char axis) { model_.Rotate(angle, axis); }

  void Scale(int scale) { model_.Scale(scale); }

  const std::vector<double>& getVertices() const {
    return model_.getVertices();
  }

  const std::vector<int>& getFaces() const { return model_.getFaces(); }

  const std::string& getFilename() const { return model_.getFilename(); }

  size_t getVertexCount() const { return model_.getVertexCount(); }

  size_t getFaceCount() const { return model_.getFaceCount(); }

 private:
  ViewerModel model_;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V_2_0_1_SRC_CONTROLLER_CONTROLLER_H_
