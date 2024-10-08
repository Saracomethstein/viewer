#ifndef C_CONTROLLER_H_
#define C_CONTROLLER_H_

#include "../backend/model.h"

typedef struct {
  bool (*LoadFromFile)(const char* filename);
  void (*Move)(int delta, char axis);
  void (*Rotate)(double angle, char axis);
  void (*Scale)(int scale);
  const double* (*getVertices)();
  const int* (*getFaces)();
  const char* (*getFilename)();
  size_t (*getVertexCount)();
  size_t (*getFaceCount)();
} Controller;

bool Controller_LoadFromFile(Controller* controller, const char* filename) {
  return controller->LoadFromFile(filename);
}

void Controller_Move(Controller* controller, int delta, char axis) {
  controller->Move(delta, axis);
}

void Controller_Rotate(Controller* controller, double angle, char axis) {
  controller->Rotate(angle, axis);
}

void Controller_Scale(Controller* controller, int scale) {
  controller->Scale(scale);
}

const double* Controller_getVertices(Controller* controller) {
  return controller->getVertices();
}

const int* Controller_getFaces(Controller* controller) {
  return controller->getFaces();
}

const char* Controller_getFilename(Controller* controller) {
  return controller->getFilename();
}

size_t Controller_getVertexCount(Controller* controller) {
  return controller->getVertexCount();
}

size_t Controller_getFaceCount(Controller* controller) {
  return controller->getFaceCount();
}

#endif  // C_CONTROLLER_H_