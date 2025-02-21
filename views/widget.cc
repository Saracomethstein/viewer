#define GL_SILENCE_DEPRECATION
#include "widget.h"

glView::glView()
    : vertexBuffer(QOpenGLBuffer::VertexBuffer),
      indexBuffer(QOpenGLBuffer::IndexBuffer),
      line_size(1.0f),
      point_size(10.0f),
      background_color(Qt::white),
      currentProjectionType(ProjectionType::Perspective),
      edgeColor(Qt::blue) {
  setFixedSize(640, 480);
}

void glView::initializeGL() {
  initializeOpenGLFunctions();
  shader_program =
      createShaderProgram(vertex_shader_source, fragment_shader_source);

  FillVertices(vertices, &dod);
  FillIndices(indices, &dod);

  v_count = dod.vertices_size;
  f_count = dod.faces_size;

  vertexBuffer.create();
  vertexBuffer.bind();
  vertexBuffer.allocate(vertices.constData(),
                        vertices.size() * sizeof(QVector3D));

  indexBuffer.create();
  indexBuffer.bind();
  indexBuffer.allocate(indices.constData(), indices.size() * sizeof(GLuint));

  vao.create();
  vao.bind();
  setupVertexAttribs();

  glClearColor(background_color.redF(), background_color.greenF(),
               background_color.blueF(), 0.0f);
  glEnable(GL_DEPTH_TEST);

  SetProjectionType(currentProjectionType);

  viewMatrix.setToIdentity();
  viewMatrix.translate(0, 0, -4);
}

void glView::paintGL() {
  glClearColor(background_color.redF(), background_color.greenF(),
               background_color.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(shader_program);

  QMatrix4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

  GLuint mvpMatrixLoc = glGetUniformLocation(shader_program, "mvpMatrix");
  glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, mvpMatrix.constData());

  GLuint edgeColorLoc = glGetUniformLocation(shader_program, "edgeColor");
  glUniform3f(edgeColorLoc, edgeColor.redF(), edgeColor.greenF(),
              edgeColor.blueF());

  GLuint vertexColorLoc = glGetUniformLocation(shader_program, "vertexColor");
  glUniform3f(vertexColorLoc, vertexColor.redF(), vertexColor.greenF(),
              vertexColor.blueF());

  GLuint drawTypeLoc = glGetUniformLocation(shader_program, "drawType");
  glUniform1i(drawTypeLoc, 0);

  glLineWidth(edgeWidth);

  if (edgeType == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  vao.bind();
  glDrawElements(GL_LINES, f_count, GL_UNSIGNED_INT, nullptr);

  glUniform1i(drawTypeLoc, 1);

  glPointSize(vertexSize);

  if (vertexDisplayType == 1) {
    glEnable(GL_POINT_SMOOTH);
  } else if (vertexDisplayType == 2) {
    glDisable(GL_POINT_SMOOTH);
  }

  if (vertexDisplayType != 0) {
    glDrawArrays(GL_POINTS, 0, v_count);
  }
}

void glView::UpdateInfoText() {
  info_text = QString(" file name: %1\n vertices: %2\n edges: %3")
                  .arg(file_path)
                  .arg(v_count)
                  .arg(f_count / 2);
}

glView::ProjectionType glView::GetProjectionType() const {
  return currentProjectionType;
}

void glView::SetProjectionType(ProjectionType type) {
  if (type == ProjectionType::Perspective) {
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  } else if (type == ProjectionType::Orthographic) {
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
  }
  currentProjectionType = type;
  update();
}

QString glView::GetInfoText() const { return info_text; }

QColor glView::GetBackgroundColor() const { return background_color; }

void glView::SetBackgroundColor(QColor new_color) {
  background_color = new_color;
  update();
}

void glView::setupVertexAttribs() {
  vertexBuffer.bind();
  QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
  f->glEnableVertexAttribArray(0);
  f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D),
                           nullptr);
  vertexBuffer.release();
  indexBuffer.bind();
  vao.release();
}

void glView::FillVertices(QVector<QVector3D>& vertices, obj_model* dod) {
  vertices.clear();
  vertices.reserve((int)dod->vertices_size);
  for (int i = 0; i < (int)dod->vertices_size * 3; i += 3) {
    vertices.append(QVector3D(dod->vertices_[i], dod->vertices_[i + 1],
                              dod->vertices_[i + 2]));
  }
}

void glView::FillIndices(QVector<GLuint>& indices, obj_model* dod) {
  indices.clear();
  indices.reserve((int)dod->faces_size);
  for (int i = 0; i < (int)dod->faces_size; i++) {
    indices.append(static_cast<GLuint>(dod->faces_[i]));
  }
}

void glView::UpdateVertexCoordinates(const QVector<QVector3D>& vertices) {
  vertexBuffer.bind();
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(QVector3D),
                  vertices.constData());
  vertexBuffer.release();
  update();
}

void glView::cleanupGL() {
  vao.destroy();
  vertexBuffer.destroy();
  indexBuffer.destroy();
}

void glView::ReinitializeOpenGL(QString& fp) {
  file_path = fp;
  free_model(&dod);
  load_obj_file(file_path.toStdString().c_str(), &dod);
  makeCurrent();
  cleanupGL();
  initializeGL();
  doneCurrent();
  UpdateInfoText();
  update();
}

GLuint glView::compileShader(GLenum type, const char* source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  return shader;
}

GLuint glView::createShaderProgram(const char* vertexSource,
                                   const char* fragmentSource) {
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  return shaderProgram;
}

void glView::MoveVertices(int delta, eCoord coord) {
  switch (coord) {
    case eCoord::X:
      move_model(&dod, delta, 'x');
      break;
    case eCoord::Y:
      move_model(&dod, delta, 'y');
      break;
    case eCoord::Z:
      move_model(&dod, delta, 'z');
      break;
  }
  FillVertices(vertices, &dod);
  UpdateVertexCoordinates(vertices);
}

void glView::RotateVertices(int delta, eCoord coord) {
  switch (coord) {
    case eCoord::X:
      rotate_model(&dod, (double)delta, 'x');
      break;
    case eCoord::Y:
      rotate_model(&dod, (double)delta, 'y');
      break;
    case eCoord::Z:
      rotate_model(&dod, (double)delta, 'z');
      break;
  }
  FillVertices(vertices, &dod);
  UpdateVertexCoordinates(vertices);
}

void glView::ScaleVertices(int delta) {
  scale_model(&dod, delta);
  FillVertices(vertices, &dod);
  UpdateVertexCoordinates(vertices);
}

glView::~glView() {
  makeCurrent();
  vertexBuffer.destroy();
  indexBuffer.destroy();
  doneCurrent();
  free_model(&dod);
  if (texture != nullptr) delete texture;
}

void glView::Render(QPainter* painter) {
  painter->beginNativePainting();
  paintGL();
  painter->endNativePainting();
}
