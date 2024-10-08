#ifndef C_MODEL_H_
#define C_MODEL_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "obj_parser.h"

int LoadFromFile(ViewerModel* model, const char* filename);
void Move(ViewerModel* model, int delta, char axis);
void Rotate(ViewerModel* model, double angle, char axis);
void Scale(ViewerModel* model, int scale);

const double* getVertices(const ViewerModel* model) { return model->vertices_; }
const int* getFaces(const ViewerModel* model) { return model->faces_; }
const char* getFilename(const ViewerModel* model) { return model->filename_; }
size_t getVertexCount(const ViewerModel* model) { return model->vertices_size; }
size_t getFaceCount(const ViewerModel* model) { return model->faces_size; }

#endif  // C_MODEL_H_
