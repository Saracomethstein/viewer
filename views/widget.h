#ifndef WIDGET_H_
#define WIDGET_H_

#include <QFileDialog>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QPainter>
#include <QPushButton>
#include <QTimerEvent>
#include <QVector3D>
#include <QWidget>

extern "C" {
#include "../backend/model.h"
#include "../backend/parser.h"
}

class glView : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  enum class ProjectionType { Perspective, Orthographic };
  enum class eCoord { X, Y, Z };
  glView();
  void MoveVertices(int delta, eCoord coord);
  void RotateVertices(int delta, eCoord coord);
  void ScaleVertices(int delta);
  void ReinitializeOpenGL(QString& fp);
  QColor GetBackgroundColor() const;

  void SetBackgroundColor(QColor);

  ProjectionType GetProjectionType() const;
  void SetProjectionType(ProjectionType type);
  QString GetInfoText() const;
  void Render(QPainter* painter);
  ~glView();

  QColor GetEdgeColor() const { return edgeColor; }
  void SetEdgeColor(const QColor& color) {
    edgeColor = color;
    update();
  }
  float GetEdgeWidth() const { return edgeWidth; }
  void SetEdgeWidth(float width) {
    edgeWidth = width;
    update();
  }
  int GetEdgeType() const { return edgeType; }
  void SetEdgeType(int type) {
    edgeType = type;
    update();
  }

  QColor GetVertexColor() const { return vertexColor; }
  void SetVertexColor(const QColor& color) {
    vertexColor = color;
    update();
  }
  float GetVertexSize() const { return vertexSize; }
  void SetVertexSize(float size) {
    vertexSize = size;
    update();
  }
  int GetVertexDisplayType() const { return vertexDisplayType; }
  void SetVertexDisplayType(int type) {
    vertexDisplayType = type;
    update();
  }

 private:
  QMatrix4x4 projectionMatrix;
  QMatrix4x4 viewMatrix;
  QMatrix4x4 modelMatrix;
  QVector<QVector3D> vertices;
  QVector<GLuint> indices;
  QOpenGLBuffer vertexBuffer;
  QOpenGLVertexArrayObject vao;
  QOpenGLBuffer indexBuffer;
  GLuint compileShader(GLenum type, const char* source);
  GLuint createShaderProgram(const char* vertexSource,
                             const char* fragmentSource);
  GLuint shader_program;

  void initializeGL() override;
  void paintGL() override;
  void setupVertexAttribs();
  void FillVertices(QVector<QVector3D>& vertices, obj_model* dod);
  void FillIndices(QVector<GLuint>& indices, obj_model* dod);
  void UpdateVertexCoordinates(const QVector<QVector3D>& vertices);
  void cleanupGL();
  void UpdateInfoText();

  QOpenGLTexture* texture;
  int v_count;
  int f_count;
  obj_model dod;
  QString file_path, info_text;
  float line_size;
  float point_size;

  QColor background_color;
  ProjectionType currentProjectionType;

  QColor edgeColor;
  float edgeWidth;
  int edgeType;

  QColor vertexColor;
  float vertexSize;
  int vertexDisplayType;

  static const inline char* vertex_shader_source = R"(
    #version 120
    attribute vec3 aPos;
    uniform mat4 mvpMatrix;
    void main() {
        gl_Position = mvpMatrix * vec4(aPos, 1.0);
    }
  )";
  static const inline char* fragment_shader_source = R"(
    #version 120
    uniform vec3 edgeColor;
    uniform vec3 vertexColor;
    uniform int drawType; // 0 - ребра, 1 - вершины

    void main() {
        if (drawType == 0) {
            gl_FragColor = vec4(edgeColor, 1.0);
        } else if (drawType == 1) {
            gl_FragColor = vec4(vertexColor, 1.0);
        }
    }
)";
 private slots:
  void openFileDialog();
};

#endif  // WIDGET_H_
