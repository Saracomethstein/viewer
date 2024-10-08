#include "test.h"

namespace s21 {

TEST(ObjParserTest, ParseValidFile) {
  ObjParser& parser = ObjParser::getInstance();
  std::vector<double> vertices;
  std::vector<int> faces;

  bool result = parser.Parse("tests/obj_files/cube.obj", vertices, faces);

  EXPECT_TRUE(result);
  EXPECT_EQ(vertices.size(), 24);
  EXPECT_EQ(faces.size(), 48);
}

TEST(ObjParserTest, ParseInvalidFile) {
  ObjParser& parser = ObjParser::getInstance();
  std::vector<double> vertices;
  std::vector<int> faces;

  bool result = parser.Parse("nonexistent.obj", vertices, faces);

  EXPECT_FALSE(result);
  EXPECT_EQ(vertices.size(), 0);
  EXPECT_EQ(faces.size(), 0);
}

TEST(ObjParserTest, ParseVertexLineValid) {
  ObjParser& parser = ObjParser::getInstance();
  std::vector<double> vertices;

  parser.ParseVertexLine("v 1.0 2.0 3.0", vertices);

  EXPECT_EQ(vertices.size(), 3);
  EXPECT_EQ(vertices[0], 1.0);
  EXPECT_EQ(vertices[1], 2.0);
  EXPECT_EQ(vertices[2], 3.0);
}

TEST(ObjParserTest, ParseVertexLineInvalidLine) {
  ObjParser& parser = ObjParser::getInstance();
  std::vector<double> vertices;

  EXPECT_THROW(parser.ParseVertexLine("v 1.0 2.0", vertices),
               std::invalid_argument);

  EXPECT_EQ(vertices.size(), 0);
}

TEST(ObjParserTest, ParseFaceLineValid) {
  ObjParser& parser = ObjParser::getInstance();
  std::vector<double> vertices = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  std::vector<int> faces;

  parser.ParseFaceLine("f 1/1/1 2/2/2 3/3/3", vertices, faces);

  EXPECT_EQ(faces.size(), 6);
  EXPECT_EQ(faces[0], 0);  // 1st vertex index - 1
  EXPECT_EQ(faces[1], 1);  // 2nd vertex index - 1
  EXPECT_EQ(faces[2], 1);  // 2nd vertex index - 1
  EXPECT_EQ(faces[3], 2);  // 3rd vertex index - 1
  EXPECT_EQ(faces[4], 2);  // 3rd vertex index - 1
  EXPECT_EQ(faces[5], 0);  // 1st vertex index - 1
}

TEST(ObjParserTest, ParseFaceLineValid1) {
  ObjParser& parser = ObjParser::getInstance();
  std::vector<double> vertices = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  std::vector<int> faces;

  parser.ParseFaceLine("f 1/1/1 2/2/2", vertices, faces);

  EXPECT_EQ(faces.size(), 4);
}

TEST(ObjParserTest, ParseFaceLineNegativeIndex) {
  ObjParser& parser = ObjParser::getInstance();
  std::vector<double> vertices = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  std::vector<int> faces;

  parser.ParseFaceLine("f -1/-1/-1 -2/-2/-2", vertices, faces);

  EXPECT_EQ(faces.size(), 4);
  EXPECT_EQ(faces[0], 1);
  EXPECT_EQ(faces[1], 0);
  EXPECT_EQ(faces[2], 0);
  EXPECT_EQ(faces[3], 1);
}

TEST(ViewerModelTest, LoadFromFileValid) {
  ViewerModel model;
  bool result = model.LoadFromFile("tests/obj_files/cube.obj");

  EXPECT_TRUE(result);
  EXPECT_EQ(model.getVertexCount(), 8);
  EXPECT_EQ(model.getFaceCount(), 48);
  EXPECT_EQ(model.getFilename(), "tests/obj_files/cube.obj");
}

TEST(ViewerModelTest, LoadFromFileInvalid) {
  ViewerModel model;
  bool result = model.LoadFromFile("nonexistent.obj");

  EXPECT_FALSE(result);
  EXPECT_EQ(model.getVertexCount(), 0);
  EXPECT_EQ(model.getFaceCount(), 0);
  EXPECT_EQ(model.getFilename(), "");
}

TEST(ViewerModelTest, MoveX) {
  ViewerModel model;
  model.LoadFromFile("tests/obj_files/pik.obj");

  const std::vector<double> verticesBefore = model.getVertices();
  model.Move(10, 'x');
  const std::vector<double> verticesAfter = model.getVertices();

  for (size_t i = 0; i < verticesBefore.size(); i += 3) {
    EXPECT_NEAR(verticesAfter[i], verticesBefore[i] + 10.0 / 20.0, 1e-6);
    EXPECT_NEAR(verticesAfter[i + 1], verticesBefore[i + 1], 1e-6);
    EXPECT_NEAR(verticesAfter[i + 2], verticesBefore[i + 2], 1e-6);
  }
}

TEST(ViewerModelTest, MoveY) {
  ViewerModel model;
  model.LoadFromFile("tests/obj_files/plant.obj");

  const std::vector<double> verticesBefore = model.getVertices();
  model.Move(10, 'y');
  const std::vector<double> verticesAfter = model.getVertices();

  for (size_t i = 0; i < verticesBefore.size(); i += 3) {
    EXPECT_NEAR(verticesAfter[i], verticesBefore[i], 1e-6);
    EXPECT_NEAR(verticesAfter[i + 1], verticesBefore[i + 1] + 10.0 / 20.0,
                1e-6);
    EXPECT_NEAR(verticesAfter[i + 2], verticesBefore[i + 2], 1e-6);
  }
}

TEST(ViewerModelTest, MoveZ) {
  ViewerModel model;
  model.LoadFromFile("tests/obj_files/skull.obj");

  const std::vector<double> verticesBefore = model.getVertices();
  model.Move(10, 'z');
  const std::vector<double> verticesAfter = model.getVertices();

  for (size_t i = 0; i < verticesBefore.size(); i += 3) {
    EXPECT_NEAR(verticesAfter[i], verticesBefore[i], 1e-6);
    EXPECT_NEAR(verticesAfter[i + 1], verticesBefore[i + 1], 1e-6);
    EXPECT_NEAR(verticesAfter[i + 2], verticesBefore[i + 2] + 10.0 / 20.0,
                1e-6);
  }
}

TEST(ViewerModelTest, RotateX) {
  ViewerModel model;
  model.LoadFromFile("tests/obj_files/cube.obj");

  const std::vector<double> verticesBefore = model.getVertices();
  model.Rotate(90, 'x');
  const std::vector<double> verticesAfter = model.getVertices();

  for (size_t i = 0; i < verticesBefore.size(); i += 3) {
    EXPECT_NEAR(verticesAfter[i], verticesBefore[i], 1e-6);
    EXPECT_NEAR(verticesAfter[i + 1], verticesBefore[i + 2], 1e-6);
    EXPECT_NEAR(verticesAfter[i + 2], -verticesBefore[i + 1], 1e-6);
  }
}

TEST(ViewerModelTest, RotateY) {
  ViewerModel model;
  model.LoadFromFile("tests/obj_files/pik.obj");

  const std::vector<double> verticesBefore = model.getVertices();
  model.Rotate(90, 'y');
  const std::vector<double> verticesAfter = model.getVertices();

  for (size_t i = 0; i < verticesBefore.size(); i += 3) {
    EXPECT_NEAR(verticesAfter[i], verticesBefore[i + 2], 1e-6);
    EXPECT_NEAR(verticesAfter[i + 1], verticesBefore[i + 1], 1e-6);
    EXPECT_NEAR(verticesAfter[i + 2], -verticesBefore[i], 1e-6);
  }
}

TEST(ViewerModelTest, RotateZ) {
  ViewerModel model;
  model.LoadFromFile("tests/obj_files/skull.obj");

  const std::vector<double> verticesBefore = model.getVertices();
  model.Rotate(90, 'z');
  const std::vector<double> verticesAfter = model.getVertices();

  for (size_t i = 0; i < verticesBefore.size(); i += 3) {
    EXPECT_NEAR(verticesAfter[i], verticesBefore[i + 1], 1e-6);
    EXPECT_NEAR(verticesAfter[i + 1], -verticesBefore[i], 1e-6);
    EXPECT_NEAR(verticesAfter[i + 2], verticesBefore[i + 2], 1e-6);
  }
}

TEST(ViewerModelTest, Scale) {
  ViewerModel model;
  model.LoadFromFile("tests/obj_files/cube.obj");

  const std::vector<double> verticesBefore = model.getVertices();
  model.Scale(10);
  const std::vector<double> verticesAfter = model.getVertices();

  double factor = 1.0 + 10.0 / 100.0;  // factor = 1.1
  for (size_t i = 0; i < verticesBefore.size(); ++i) {
    EXPECT_NEAR(verticesAfter[i], verticesBefore[i] * factor, 1e-6);
  }
}

TEST(ObjParserTest, ParseFaceLineInvalidVertexIndex) {
  ObjParser& parser = ObjParser::getInstance();
  std::vector<double> vertices = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  std::vector<int> faces;

  EXPECT_THROW(parser.ParseFaceLine("f 1/1/1 a/a/a 3/3/3", vertices, faces),
               std::invalid_argument);

  EXPECT_EQ(faces.size(), 0);
}

}  // namespace s21