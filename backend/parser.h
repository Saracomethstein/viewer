#ifndef C_PARSER_H_
#define C_PARSER_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  double* vertices_;
  int* faces_;
  int vertices_size;
  int faces_size;
  char* filename_;
} obj_model;

enum STATUS { OK, ERROR };

int load_obj_file(const char* filename, obj_model* model);
int parse_obj_file(FILE* file, obj_model* model);
int allocate_memory_for_model(obj_model* model);
void calculate_vertex_and_face_size(FILE* file, obj_model* model);
void free_model(obj_model* model);

#endif  // C_PARSER_H_
