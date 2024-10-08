#include "obj_parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Parse(ViewerModel* parser) {
  FILE* file = fopen(parser->filename_, "r");
  if (!file) {
    return false;
  }

  char line[256];
  parser->vertices_size = 0;
  parser->faces_size = 0;
  parser->vertices_ = NULL;
  parser->faces_ = NULL;

  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "v ", 2) == 0) {
      ParseVertexLine(line, parser);
    } else if (strncmp(line, "f ", 2) == 0) {
      ParseFaceLine(line, parser);
    }
  }

  fclose(file);
  return true;
}

void ParseVertexLine(const char* line, ViewerModel* parser) {
  double x, y, z;
  sscanf(line + 2, "%lf %lf %lf", &x, &y, &z);

  parser->vertices_ =
      realloc(parser->vertices_, (parser->vertices_size + 3) * sizeof(double));
  (parser->vertices_)[parser->vertices_size] = x;
  (parser->vertices_)[parser->vertices_size + 1] = y;
  (parser->vertices_)[parser->vertices_size + 2] = z;
  parser->vertices_size += 3;
}

void ParseFaceLine(const char* line, ViewerModel* parser) {
  char* token;
  char* line_copy = strdup(line + 2);
  int first_index = -1, last_index = -1;

  token = strtok(line_copy, " ");
  while (token != NULL) {
    int vertex_index = atoi(token) - 1;

    if (vertex_index < 0) {
      vertex_index += parser->vertices_size / 3;
    }

    if (first_index == -1) {
      first_index = vertex_index;
    }

    if (last_index != -1) {
      parser->faces_ =
          realloc(parser->faces_, (parser->faces_size + 2) * sizeof(int));
      (parser->faces_)[parser->faces_size] = last_index;
      (parser->faces_)[parser->faces_size + 1] = vertex_index;
      parser->faces_size += 2;
    }

    last_index = vertex_index;
    token = strtok(NULL, " ");
  }

  if (first_index != -1 && last_index != -1) {
    parser->faces_ =
        realloc(parser->faces_, (parser->faces_size + 2) * sizeof(int));
    (parser->faces_)[parser->faces_size] = last_index;
    (parser->faces_)[parser->faces_size + 1] = first_index;
    parser->faces_size += 2;
  }

  free(line_copy);
}
