#ifndef CPP4_3DVIEWER_V_2_0_1_SRC_BACKEND_OBJ_PARSER_H_
#define CPP4_3DVIEWER_V_2_0_1_SRC_BACKEND_OBJ_PARSER_H_

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {

class ObjParser {
 public:
  static ObjParser& getInstance() {
    static ObjParser instance;
    return instance;
  }

  bool Parse(const std::string& filename, std::vector<double>& vertices,
             std::vector<int>& faces);
  void ParseVertexLine(const std::string& line, std::vector<double>& vertices);
  void ParseFaceLine(const std::string& line,
                     const std::vector<double>& vertices,
                     std::vector<int>& faces);

 private:
  ObjParser() = default;
  ~ObjParser() = default;
  ObjParser(const ObjParser&) = delete;
  ObjParser& operator=(const ObjParser&) = delete;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V_2_0_1_SRC_BACKEND_OBJ_PARSER_H_
