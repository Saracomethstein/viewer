#include "model.h"

#include <cmath>
#include <iostream>

namespace s21 {

bool ViewerModel::LoadFromFile(const std::string& filename) {
  faces_.clear();
  vertices_.clear();
  bool result = ObjParser::getInstance().Parse(filename, vertices_, faces_);
  if (result) {
    filename_ = filename;
  }
  return result;
}

void ViewerModel::Move(int delta, char axis) {
  for (size_t i = 0; i < vertices_.size(); i += 3) {
    switch (axis) {
      case 'x':
        vertices_[i] += delta / 20.0;
        break;
      case 'y':
        vertices_[i + 1] += delta / 20.0;
        break;
      case 'z':
        vertices_[i + 2] += delta / 20.0;
        break;
    }
  }
}

void ViewerModel::Rotate(double angle, char axis) {
  double angle_in_rad = angle * M_PI / 180.0;
  double cos_a = cos(angle_in_rad);
  double sin_a = sin(angle_in_rad);
  for (size_t i = 0; i < vertices_.size(); i += 3) {
    double x = vertices_[i];
    double y = vertices_[i + 1];
    double z = vertices_[i + 2];
    switch (axis) {
      case 'x':
        vertices_[i + 1] = y * cos_a + z * sin_a;
        vertices_[i + 2] = -y * sin_a + z * cos_a;
        break;
      case 'y':
        vertices_[i] = x * cos_a + z * sin_a;
        vertices_[i + 2] = -x * sin_a + z * cos_a;
        break;
      case 'z':
        vertices_[i] = x * cos_a + y * sin_a;
        vertices_[i + 1] = -x * sin_a + y * cos_a;
        break;
    }
  }
}

void ViewerModel::Scale(int scale) {
  double factor = 1.0 + 1.0 / (100.0 / scale);
  for (auto& vertex : vertices_) {
    vertex *= factor;
  }
}

}  // namespace s21
