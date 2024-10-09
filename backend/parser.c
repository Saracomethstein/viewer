#include "parser.h"

int load_obj_file(const char* filename, obj_model* model) {
  if (!filename || !model) {
    return ERROR;
  }

  int status = OK;
  FILE* file = fopen(filename, "r");
  if (file) {
    calculate_vertex_and_face_size(file, model);
    status = allocate_memory_for_model(model);
    if (status != ERROR) {
      fseek(file, 0, SEEK_SET);
      status = parse_obj_file(file, model);
    }
    fclose(file);
  } else {
    status = ERROR;
  }

  return status;
}

int parse_obj_file(FILE* file, obj_model* model) {
  int status = OK;
  char* line = NULL;
  size_t len = 0;
  size_t vertexIndex = 0;
  size_t faceIndex = 0;

  while (getline(&line, &len, file) != -1) {
    if (strncmp(line, "v ", 2) == 0) {
      double x, y, z;
      sscanf(line, "v %lf %lf %lf", &x, &y, &z);
      model->vertices_[vertexIndex * 3] = x;
      model->vertices_[vertexIndex * 3 + 1] = y;
      model->vertices_[vertexIndex * 3 + 2] = z;
      vertexIndex++;
    } else if (strncmp(line, "f ", 2) == 0) {
      char* token = strtok(line + 2, " ");
      int vertexIdx;
      int firstVertex = -1;
      int lastVertex = -1;

      while (token != NULL) {
        if (sscanf(token, "%d/%*d/%*d", &vertexIdx) == 1) {
          if (vertexIdx < 0) {
            vertexIdx += vertexIndex + 1;
          }
          vertexIdx--;

          if (firstVertex == -1) {
            firstVertex = vertexIdx;
          }

          if (lastVertex != -1) {
            model->faces_[faceIndex++] = lastVertex;
            model->faces_[faceIndex++] = vertexIdx;
          }

          lastVertex = vertexIdx;
        }
        token = strtok(NULL, " ");
      }

      if (firstVertex != -1 && lastVertex != -1) {
        model->faces_[faceIndex++] = lastVertex;
        model->faces_[faceIndex++] = firstVertex;
      }
    }
  }

  if (line) {
    free(line);
  }

  return status;
}

int allocate_memory_for_model(obj_model* model) {
  int status = OK;
  model->vertices_ = (double*)malloc(model->vertices_size * 3 * sizeof(double));
  model->faces_ = (int*)malloc(model->faces_size * 2 * sizeof(int));

  if (!model->vertices_ || !model->faces_) {
    status = ERROR;
  }
  if (status == ERROR) {
    if (model->vertices_) free(model->vertices_);
    if (model->faces_) free(model->faces_);
  }
  return status;
}

void calculate_vertex_and_face_size(FILE* file, obj_model* model) {
  size_t vertexCount = 0;
  size_t faceCount = 0;
  char* line = NULL;
  size_t len = 0;

  while (getline(&line, &len, file) != -1) {
    line[strcspn(line, "\n")] = '\0';

    if (line[0] == '#' || line[0] == '\0') {
      continue;
    }

    if (strncmp(line, "v ", 2) == 0) {
      vertexCount++;
    } else if (strncmp(line, "f ", 2) == 0) {
      size_t vertexInFace = 0;
      char* token = strtok(line + 2, " ");

      while (token != NULL) {
        vertexInFace++;
        token = strtok(NULL, " ");
      }

      faceCount += vertexInFace;
    }
  }

  free(line);

  model->vertices_size = vertexCount;
  model->faces_size = faceCount * 2;
}

void free_model(obj_model* model) {
  if (model) {
    if (model->faces_) {
      free(model->faces_);
      model->faces_ = NULL;
    }
    if (model->vertices_) {
      free(model->vertices_);
      model->vertices_ = NULL;
    }
  }
}
