#ifndef CPP4_3DVIEWER_V_2_0_1_SRC_VIEW_WIDGET_H_
#define CPP4_3DVIEWER_V_2_0_1_SRC_VIEW_WIDGET_H_

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

#include "../controller/controller.h"
#include "projection_strategy.h"

namespace s21 {

class GlView : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  enum class ProjectionType { Perspective, Orthographic };
  enum class eCoord { X, Y, Z };
  GlView(s21::Controller& controller);
  void ReinitializeOpenGL(QString& fp);
  QColor GetBackgroundColor() const;

  void SetBackgroundColor(QColor);

  ProjectionType GetProjectionType() const;
  void SetProjectionType(ProjectionType type);
  QString GetInfoText() const;
  void Render(QPainter* painter);
  ~GlView();

  QColor GetEdgeColor() const { return edge_color_; }
  void SetEdgeColor(const QColor& color) {
    edge_color_ = color;
    update();
  }
  float GetEdgeWidth() const { return edge_width_; }
  void SetEdgeWidth(float width) {
    edge_width_ = width;
    update();
  }
  int GetEdgeType() const { return edge_type_; }
  void SetEdgeType(int type) {
    edge_type_ = type;
    update();
  }

  QColor GetVertexColor() const { return vertex_color_; }
  void SetVertexColor(const QColor& color) {
    vertex_color_ = color;
    update();
  }
  float GetVertexSize() const { return vertex_size_; }
  void SetVertexSize(float size) {
    vertex_size_ = size;
    update();
  }
  int GetVertexDisplayType() const { return vertex_display_type_; }
  void SetVertexDisplayType(int type) {
    vertex_display_type_ = type;
    update();
  }

  void UpdateVertices();

 private:
  QMatrix4x4 projection_matrix_;
  QMatrix4x4 view_matrix_;
  QMatrix4x4 model_matrix_;
  QVector<QVector3D> vertices_;
  QVector<GLuint> indices_;
  QOpenGLBuffer vertex_buffer_;
  QOpenGLVertexArrayObject vao_;
  QOpenGLBuffer index_buffer_;
  GLuint CompileShader(GLenum type, const char* source);
  GLuint CreateShaderProgram(const char* vertexSource,
                             const char* fragmentSource);
  GLuint shader_program;

  void initializeGL() override;
  void paintGL() override;
  void setupVertexAttribs();
  void FillVertices(QVector<QVector3D>& vertices,
                    const std::vector<double>& modelVertices);
  void FillIndices(QVector<GLuint>& indices,
                   const std::vector<int>& modelFaces);
  void UpdateVertexCoordinates(const QVector<QVector3D>& vertices);
  void cleanupGL();
  void UpdateInfoText();

  QOpenGLTexture* texture_;
  int v_count_;
  int f_count_;
  QString file_path_, info_text_;  // Поле для хранения пути к файлу

  QColor background_color_;
  ProjectionType current_projection_type_;

  // Для ребер
  QColor edge_color_;
  float edge_width_;
  int edge_type_;  // 0 - сплошная, 1 - штриховая

  // Для вершин
  QColor vertex_color_;
  float vertex_size_;
  int vertex_display_type_;  // 0 - отсутствует, 1 - круг, 2 - квадрат

  s21::Controller& controller_;
  std::unique_ptr<ProjectionStrategy> projection_strategy_;

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
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V_2_0_1_SRC_VIEW_WIDGET_H_
