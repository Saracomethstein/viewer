#include "parser.h"

int parse(const char* filename, obj_model* parser) {
  FILE* file = fopen(filename, "r");
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
      parse_vertex_line(line, parser);
    } else if (strncmp(line, "f ", 2) == 0) {
      parse_face_line(line, parser);
    }
  }

  fclose(file);
  return true;
}

void parse_vertex_line(const char* line, obj_model* parser) {
  double x, y, z;
  sscanf(line + 2, "%lf %lf %lf", &x, &y, &z);

  parser->vertices_ =
      realloc(parser->vertices_, (parser->vertices_size + 3) * sizeof(double));
  (parser->vertices_)[parser->vertices_size] = x;
  (parser->vertices_)[parser->vertices_size + 1] = y;
  (parser->vertices_)[parser->vertices_size + 2] = z;
  parser->vertices_size += 3;
}

void parse_face_line(const char* line, obj_model* parser) {
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

void free_obj_model(obj_model* parser) {
  if (parser) {
    if (parser->faces_) {
      free(parser->faces_);
      parser->faces_ = NULL;
    }
    if (parser->vertices_) {
      free(parser->vertices_);
      parser->vertices_ = NULL;
    }
  }
}
