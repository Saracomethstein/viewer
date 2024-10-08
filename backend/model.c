#include "model.h"

#include <math.h>
#include <stdio.h>

#include "obj_parser.h"

int LoadFromFile(ViewerModel* model, const char* filename) {
  free(model->vertices_);
  free(model->faces_);
  model->vertices_ = NULL;
  model->faces_ = NULL;
  model->vertices_size = 0;
  model->faces_size = 0;
  bool result = Parse(model);
  if (result) {
    model->filename_ = filename;
  }
  return result;
}

void Move(ViewerModel* model, int delta, char axis) {
  for (size_t i = 0; i < model->vertices_size; i += 3) {
    switch (axis) {
      case 'x':
        model->vertices_[i] += delta / 20.0;
        break;
      case 'y':
        model->vertices_[i + 1] += delta / 20.0;
        break;
      case 'z':
        model->vertices_[i + 2] += delta / 20.0;
        break;
    }
  }
}

void Rotate(ViewerModel* model, double angle, char axis) {
  double angle_in_rad = angle * M_PI / 180.0;
  double cos_a = cos(angle_in_rad);
  double sin_a = sin(angle_in_rad);
  for (size_t i = 0; i < model->vertices_size; i += 3) {
    double x = model->vertices_[i];
    double y = model->vertices_[i + 1];
    double z = model->vertices_[i + 2];
    switch (axis) {
      case 'x':
        model->vertices_[i + 1] = y * cos_a + z * sin_a;
        model->vertices_[i + 2] = -y * sin_a + z * cos_a;
        break;
      case 'y':
        model->vertices_[i] = x * cos_a + z * sin_a;
        model->vertices_[i + 2] = -x * sin_a + z * cos_a;
        break;
      case 'z':
        model->vertices_[i] = x * cos_a + y * sin_a;
        model->vertices_[i + 1] = -x * sin_a + y * cos_a;
        break;
    }
  }
}

void Scale(ViewerModel* model, int scale) {
  double factor = 1.0 + 1.0 / (100.0 / scale);
  for (size_t i = 0; i < model->vertices_size; i++) {
    model->vertices_[i] *= factor;
  }
}