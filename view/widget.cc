#define GL_SILENCE_DEPRECATION
#include "widget.h"

namespace s21 {

GlView::GlView(s21::Controller& controller)
    : vertex_buffer_(QOpenGLBuffer::VertexBuffer),
      index_buffer_(QOpenGLBuffer::IndexBuffer),
      background_color_(Qt::white),
      current_projection_type_(ProjectionType::Perspective),
      edge_color_(Qt::blue),
      controller_(controller) {
  setFixedSize(640, 480);
}

void GlView::initializeGL() {
  initializeOpenGLFunctions();
  shader_program =
      CreateShaderProgram(vertex_shader_source, fragment_shader_source);

  const std::vector<double>& modelVertices = controller_.getVertices();
  const std::vector<int>& modelFaces = controller_.getFaces();

  FillVertices(vertices_, modelVertices);
  FillIndices(indices_, modelFaces);

  v_count_ = controller_.getVertexCount();
  f_count_ = controller_.getFaceCount();

  vertex_buffer_.create();
  vertex_buffer_.bind();
  vertex_buffer_.allocate(vertices_.constData(),
                          vertices_.size() * sizeof(QVector3D));

  index_buffer_.create();
  index_buffer_.bind();
  index_buffer_.allocate(indices_.constData(),
                         indices_.size() * sizeof(GLuint));

  vao_.create();
  vao_.bind();
  setupVertexAttribs();

  glClearColor(background_color_.redF(), background_color_.greenF(),
               background_color_.blueF(), 0.0f);
  glEnable(GL_DEPTH_TEST);

  // Установка матрицы проекции
  SetProjectionType(current_projection_type_);

  // Установка матрицы вида
  view_matrix_.setToIdentity();
  view_matrix_.translate(0, 0, -4);  // start point of view
}

void GlView::paintGL() {
  glClearColor(background_color_.redF(), background_color_.greenF(),
               background_color_.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(shader_program);

  // Задание матрицы проекции и вида
  QMatrix4x4 mvpMatrix = projection_matrix_ * view_matrix_ * model_matrix_;

  GLuint mvpMatrixLoc = glGetUniformLocation(shader_program, "mvpMatrix");
  glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, mvpMatrix.constData());

  // Установка цвета ребер
  GLuint edgeColorLoc = glGetUniformLocation(shader_program, "edgeColor");
  glUniform3f(edgeColorLoc, edge_color_.redF(), edge_color_.greenF(),
              edge_color_.blueF());

  // Установка цвета вершин
  GLuint vertexColorLoc = glGetUniformLocation(shader_program, "vertexColor");
  glUniform3f(vertexColorLoc, vertex_color_.redF(), vertex_color_.greenF(),
              vertex_color_.blueF());

  // Установка drawType для ребер
  GLuint drawTypeLoc = glGetUniformLocation(shader_program, "drawType");
  glUniform1i(drawTypeLoc, 0);

  // Установка толщины ребер
  glLineWidth(edge_width_);

  // Установка типа ребер (сплошная/штриховая)
  if (edge_type_ == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  vao_.bind();
  glDrawElements(GL_LINES, f_count_, GL_UNSIGNED_INT, nullptr);

  // Установка drawType для вершин
  glUniform1i(drawTypeLoc, 1);

  // Установка размера вершин
  glPointSize(vertex_size_);

  // Установка типа отображения вершин
  if (vertex_display_type_ == 1) {
    // Круг
    glEnable(GL_POINT_SMOOTH);
  } else if (vertex_display_type_ == 2) {
    // Квадрат
    glDisable(GL_POINT_SMOOTH);
  }

  if (vertex_display_type_ != 0) {
    glDrawArrays(GL_POINTS, 0, v_count_);
  }
}

void GlView::UpdateInfoText() {
  info_text_ = QString(" file name: %1\n vertices: %2\n edges: %3")
                   .arg(file_path_)
                   .arg(v_count_)
                   .arg(f_count_ / 2);
}

GlView::ProjectionType GlView::GetProjectionType() const {
  return current_projection_type_;
}

void GlView::SetProjectionType(ProjectionType type) {
  if (type == ProjectionType::Perspective) {
    projection_strategy_ = std::make_unique<PerspectiveProjection>();
  } else if (type == ProjectionType::Orthographic) {
    projection_strategy_ = std::make_unique<OrthographicProjection>();
  }
  projection_strategy_->ApplyProjection(projection_matrix_);
  current_projection_type_ = type;
  update();
}

QString GlView::GetInfoText() const { return info_text_; }

QColor GlView::GetBackgroundColor() const { return background_color_; }

void GlView::SetBackgroundColor(QColor new_color) {
  background_color_ = new_color;
  update();
}

void GlView::setupVertexAttribs() {
  vertex_buffer_.bind();
  QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
  f->glEnableVertexAttribArray(0);
  f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D),
                           nullptr);
  vertex_buffer_.release();

  index_buffer_.bind();
  vao_.release();
}

void GlView::FillVertices(QVector<QVector3D>& vertices,
                          const std::vector<double>& modelVertices) {
  vertices.clear();
  vertices.reserve(modelVertices.size() / 3);

  for (size_t i = 0; i < modelVertices.size(); i += 3) {
    vertices.append(QVector3D(modelVertices[i], modelVertices[i + 1],
                              modelVertices[i + 2]));
  }
}

void GlView::FillIndices(QVector<GLuint>& indices,
                         const std::vector<int>& modelFaces) {
  indices.clear();
  indices.reserve(modelFaces.size());

  for (size_t i = 0; i < modelFaces.size(); i++) {
    indices.append(static_cast<GLuint>(modelFaces[i]));
  }
}

void GlView::UpdateVertexCoordinates(const QVector<QVector3D>& vertices) {
  vertex_buffer_.bind();
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(QVector3D),
                  vertices.constData());
  vertex_buffer_.release();

  update();
}

void GlView::cleanupGL() {
  vao_.destroy();
  vertex_buffer_.destroy();
  index_buffer_.destroy();
}

void GlView::ReinitializeOpenGL(QString& fp) {
  file_path_ = fp;
  makeCurrent();
  cleanupGL();
  initializeGL();
  doneCurrent();
  UpdateInfoText();
  update();
}

GLuint GlView::CompileShader(GLenum type, const char* source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  return shader;
}

GLuint GlView::CreateShaderProgram(const char* vertexSource,
                                   const char* fragmentSource) {
  GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
  GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // удаляем шейдеры после линковки тк они больше не нужны
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

GlView::~GlView() {
  makeCurrent();
  vertex_buffer_.destroy();
  index_buffer_.destroy();
  doneCurrent();
  if (texture_ != nullptr) delete texture_;
}

void GlView::Render(QPainter* painter) {
  // Рендерим содержимое OpenGL контекста в QPainter
  painter->beginNativePainting();

  paintGL();

  painter->endNativePainting();
}

void GlView::UpdateVertices() {
  FillVertices(vertices_, controller_.getVertices());
  UpdateVertexCoordinates(vertices_);
}
}  // namespace s21
