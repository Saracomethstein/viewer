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
  glView();
  enum class eCoord { X, Y, Z };
  void MoveVertices(int delta, eCoord coord);
  void RotateVertices(int delta, eCoord coord);
  void ScaleVertices(int delta);
  void ReinitializeOpenGL(QString fp);
  QString GetInfoText() const;
  ~glView();

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
  void FillVertices(QVector<QVector3D>& vertices, dot_obj_data* dod);
  void FillIndices(QVector<GLuint>& indices, dot_obj_data* dod);
  void UpdateVertexCoordinates(const QVector<QVector3D>& vertices);
  void cleanupGL();
  void UpdateInfoText();

  QOpenGLTexture* texture;
  int v_count;
  int f_count;
  dot_obj_data dod;
  QString file_path, info_text;
  float line_size;
  float point_size;
  QColor line_color;
  QColor background_color;

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
    uniform vec3 lineColor;
    void main() {
        gl_FragColor = vec4(lineColor, 1.0);
    }
  )";

 private slots:
  void openFileDialog();
};

#endif  // WIDGET_H_
