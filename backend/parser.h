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

enum MOVEMENT { MOVE_X, MOVE_Y, MOVE_Z };

int parse(const char* filename, obj_model* parser);
void parse_vertex_line(const char* line, obj_model* parser);
void parse_face_line(const char* line, obj_model* parser);
void free_obj_model(obj_model* parser);

#endif  // C_PARSER_H_
