#ifndef C_OBJ_PARSER_H_
#define C_OBJ_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  double* vertices_;
  int* faces_;
  char* filename_;
  int vertices_size;
  int faces_size;
} ViewerModel;

ViewerModel* getInstance();
int Parse(ViewerModel* parser);
void ParseVertexLine(const char* line, ViewerModel* parser);
void ParseFaceLine(const char* line, ViewerModel* parser);

#endif  // C_OBJ_PARSER_H_
