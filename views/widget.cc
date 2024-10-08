#include "widget.h"

glView::glView()
    : vertexBuffer(QOpenGLBuffer::VertexBuffer),
      indexBuffer(QOpenGLBuffer::IndexBuffer),
      line_size(1.0f),
      point_size(10.0f),
      line_color(Qt::blue),
      background_color(Qt::white) {
  setFixedSize(640, 480);
}

void glView::initializeGL() {
  initializeOpenGLFunctions();
  shader_program =
      createShaderProgram(vertex_shader_source, fragment_shader_source);

  FillVertices(vertices, &dod);
  FillIndices(indices, &dod);

  v_count = dod.v_count;
  f_count = dod.f_count;

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

  // Установка матрицы проекции
  projectionMatrix.setToIdentity();
  projectionMatrix.perspective(45.0f, 3.0f / 3.0f, 0.1f, 100.0f);

  // Установка матрицы вида
  viewMatrix.setToIdentity();
  viewMatrix.translate(0, 0, -4);  // start point of view
}

void glView::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(shader_program);

  // Задание матрицы проекции и вида
  QMatrix4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

  GLuint mvpMatrixLoc = glGetUniformLocation(shader_program, "mvpMatrix");
  glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, mvpMatrix.constData());

  // Передача цвета в шейдер
  GLuint colorLoc = glGetUniformLocation(shader_program, "lineColor");
  glUniform3f(colorLoc, line_color.redF(), line_color.greenF(),
              line_color.blueF());

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(mvpMatrix.constData());

  glLineWidth(line_size);
  glPointSize(point_size);

  vao.bind();

  glDrawElements(GL_LINES, f_count, GL_UNSIGNED_INT, nullptr);
  glDrawArrays(GL_POINTS, 0, v_count);

  update();
}

void glView::UpdateInfoText() {
  info_text = QString(" file name: %1\n vertices: %2\n edges: %3")
                  .arg(file_path)
                  .arg(v_count)
                  .arg(f_count / 2);
}

QString glView::GetInfoText() const { return info_text; }

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

void glView::FillVertices(QVector<QVector3D>& vertices, dot_obj_data* dod) {
  vertices.clear();
  vertices.reserve((int)dod->v_count);

  for (int i = 0; i < (int)dod->v_count * 3; i += 3) {
    vertices.append(QVector3D(dod->vertices[i], dod->vertices[i + 1],
                              dod->vertices[i + 2]));
  }
}

void glView::FillIndices(QVector<GLuint>& indices, dot_obj_data* dod) {
  indices.clear();
  indices.reserve((int)dod->f_count);

  for (int i = 0; i < (int)dod->f_count; i++) {
    indices.append(static_cast<GLuint>(dod->faces[i]));
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

void glView::ReinitializeOpenGL(QString fp) {
  file_path = fp;
  free_obj_model(&dod);
  parse(file_path.toStdString().c_str(), &dod);
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

  // удаляем шейдеры после линковки тк они больше не нужны
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

void glView::MoveVertices(int delta, eCoord coord) {
  switch (coord) {
    case eCoord::X:
      move(&dod, delta, 'x');
      break;
    case eCoord::Y:
      move(&dod, delta, 'y');
      break;
    case eCoord::Z:
      move(&dod, delta, 'z');
      break;
  }
  FillVertices(vertices, &dod);
  UpdateVertexCoordinates(vertices);
}

void glView::RotateVertices(int delta, eCoord coord) {
  switch (coord) {
    case eCoord::X:
      rotate(&dod, (double)delta, 'x');
      break;
    case eCoord::Y:
      rotate(&dod, (double)delta, 'y');
      break;
    case eCoord::Z:
      rotate(&dod, (double)delta, 'z');
      break;
  }
  FillVertices(vertices, &dod);
  UpdateVertexCoordinates(vertices);
}

void glView::ScaleVertices(int delta) {
  scale(&dod, delta);
  FillVertices(vertices, &dod);
  UpdateVertexCoordinates(vertices);
}

glView::~glView() {
  makeCurrent();
  vertexBuffer.destroy();
  indexBuffer.destroy();
  doneCurrent();
  free_obj_model(&dod);
  if (texture != nullptr) delete texture;
}
