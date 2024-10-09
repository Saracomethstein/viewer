#include "model.h"

void move_model(obj_model* model, int delta, char axis) {
  for (int i = 0; i < model->vertices_size * 3; i += 3) {
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

void scale_model(obj_model* model, int scale) {
  for (size_t i = 0; i < model->vertices_size * 3; i++) {
    model->vertices_[i] *=
        ((double)1 + (double)1 / ((double)100 / ((double)scale)));
  }
}

void rotate_model(obj_model* model, double angle, char axis) {
  double angle_in_rad = angle * M_PI / 180.0;
  double cos_a = cos(angle_in_rad);
  double sin_a = sin(angle_in_rad);
  for (int i = 0; i < model->vertices_size * 3; i += 3) {
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
