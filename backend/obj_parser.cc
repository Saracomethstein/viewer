#include "obj_parser.h"

namespace s21 {

bool ObjParser::Parse(const std::string& filename,
                      std::vector<double>& vertices, std::vector<int>& faces) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.substr(0, 2) == "v ") {
      ParseVertexLine(line, vertices);
    } else if (line.substr(0, 2) == "f ") {
      ParseFaceLine(line, vertices, faces);
    }
  }

  return true;
}

void ObjParser::ParseVertexLine(const std::string& line,
                                std::vector<double>& vertices) {
  std::istringstream iss(line.substr(2));
  double x, y, z;

  if (!(iss >> x >> y >> z)) {
    throw std::invalid_argument("Invalid vertex line: " + line);
  }

  vertices.push_back(x);
  vertices.push_back(y);
  vertices.push_back(z);
}

void ObjParser::ParseFaceLine(const std::string& line,
                              const std::vector<double>& vertices,
                              std::vector<int>& faces) {
  std::istringstream iss(line.substr(2));
  std::string token;
  int first_index = -1, last_index = -1;

  while (iss >> token) {
    std::istringstream token_iss(token);
    std::string vertex_index_str;
    std::getline(token_iss, vertex_index_str, '/');

    int vertex_index = 0;
    try {
      vertex_index = std::stoi(vertex_index_str);
    } catch (const std::invalid_argument&) {
      // Очищаем вектор faces, так как обнаружена ошибка
      faces.clear();
      throw std::invalid_argument("Invalid vertex index: " + vertex_index_str);
    }

    if (vertex_index < 0) {
      vertex_index += vertices.size() / 3 + 1;
    }
    vertex_index--;

    if (first_index == -1) {
      first_index = vertex_index;
    }

    if (last_index != -1) {
      faces.push_back(last_index);
      faces.push_back(vertex_index);
    }

    last_index = vertex_index;
  }

  if (first_index != -1 && last_index != -1) {
    faces.push_back(last_index);
    faces.push_back(first_index);
  }
}

}  // namespace s21
