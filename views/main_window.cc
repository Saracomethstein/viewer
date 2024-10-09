#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      my_gl(new glView()),
      but(new QPushButton("Upload model", this)),
      scale_sub_but(new QPushButton("-", this)),
      scale_add_but(new QPushButton("+", this)),
      background_color_but(new QPushButton("Background color", this)),
      projection_but(new QPushButton(this)),
      edgeColorButton(new QPushButton("Edge Color", this)),
      vertexColorButton(new QPushButton("Vertex Color", this)),
      save_but(new QPushButton("Save Image", this)),
      record_but(new QPushButton("Record Screencast", this)),
      show_color_but(new QPushButton("Show Color", this)),
      show_rotate_but(new QPushButton("Show Rotate", this)),
      show_move_but(new QPushButton("Show Move", this)),
      reset_defaults_but(new QPushButton("Reset to Defaults", this)),
      slrotate_x(new QScrollBar(Qt::Horizontal, this)),
      slrotate_y(new QScrollBar(Qt::Horizontal, this)),
      slrotate_z(new QScrollBar(Qt::Horizontal, this)),
      slmove_x(new QScrollBar(Qt::Horizontal, this)),
      slmove_y(new QScrollBar(Qt::Horizontal, this)),
      slmove_z(new QScrollBar(Qt::Horizontal, this)),
      info_label(new QLabel(this)),
      label_x_sub(new QLabel("X-", this)),
      label_y_sub(new QLabel("Y-", this)),
      label_z_sub(new QLabel("Z-", this)),
      label_x_add(new QLabel("X+", this)),
      label_y_add(new QLabel("Y+", this)),
      label_z_add(new QLabel("Z+", this)),
      label_x_sub1(new QLabel("X-", this)),
      label_y_sub1(new QLabel("Y-", this)),
      label_z_sub1(new QLabel("Z-", this)),
      label_x_add1(new QLabel("X+", this)),
      label_y_add1(new QLabel("Y+", this)),
      label_z_add1(new QLabel("Z+", this)),
      rot_x_layout(new QHBoxLayout()),
      rot_y_layout(new QHBoxLayout()),
      rot_z_layout(new QHBoxLayout()),
      move_x_layout(new QHBoxLayout()),
      move_y_layout(new QHBoxLayout()),
      move_z_layout(new QHBoxLayout()),
      scale_layout(new QHBoxLayout()),
      color_layout(new QGridLayout()),
      central_widget(new QWidget(this)),
      main_layout(new QVBoxLayout(central_widget)),
      rotate_layout(new QVBoxLayout()),
      move_layout(new QVBoxLayout()),
      scale_group(new QGroupBox("Scale", this)),
      rotate_group(new QGroupBox("Rotate", this)),
      move_group(new QGroupBox("Move", this)),
      color_group(new QGroupBox("Color", this)),
      left_spacer(new QSpacerItem(20, 20, QSizePolicy::Expanding,
                                  QSizePolicy::Minimum)),
      right_spacer(new QSpacerItem(20, 20, QSizePolicy::Expanding,
                                   QSizePolicy::Minimum)),
      edgeWidthSlider(new QSlider(Qt::Horizontal, this)),
      vertexSizeSlider(new QSlider(Qt::Horizontal, this)),
      edgeTypeComboBox(new QComboBox(this)),
      vertexDisplayTypeComboBox(new QComboBox(this)),
      gif(nullptr),
      timer(new QTimer(this)),
      originalSize(this->size()),
      settings("HappyThreeFriends", "3DViewerApp"),
      frameCount(0),
      prev_x_rot(50),
      prev_y_rot(50),
      prev_z_rot(50),
      prev_x_move(50),
      prev_y_move(50),
      prev_z_move(50),
      scale_val(10) {
  setWindowTitle("3DViewer_v1.0");
  setFixedWidth(fixed_size);

  edgeWidthSlider->setRange(1, 10);
  vertexSizeSlider->setRange(1, 20);

  edgeTypeComboBox->addItem("Solid");
  edgeTypeComboBox->addItem("Dashed");

  vertexDisplayTypeComboBox->addItem("None");
  vertexDisplayTypeComboBox->addItem("Circle");
  vertexDisplayTypeComboBox->addItem("Square");

  color_layout->addWidget(new QLabel("Edge Color:"), 0, 0);
  color_layout->addWidget(edgeColorButton, 0, 1);
  color_layout->addWidget(new QLabel("Vertex Color:"), 1, 0);
  color_layout->addWidget(vertexColorButton, 1, 1);
  color_layout->addWidget(new QLabel("Backgroud Color:"), 2, 0);
  color_layout->addWidget(background_color_but, 2, 1);
  color_layout->addWidget(new QLabel("Edge Width:"), 3, 0);
  color_layout->addWidget(edgeWidthSlider, 3, 1);
  color_layout->addWidget(new QLabel("Vertex Size:"), 4, 0);
  color_layout->addWidget(vertexSizeSlider, 4, 1);
  color_layout->addWidget(new QLabel("Edge Type:"), 5, 0);
  color_layout->addWidget(edgeTypeComboBox, 5, 1);
  color_layout->addWidget(new QLabel("Vertex Display:"), 6, 0);
  color_layout->addWidget(vertexDisplayTypeComboBox, 6, 1);

  UpdateProjectionButtonText();

  slrotate_x->setValue(prev_x_rot);
  slrotate_y->setValue(prev_y_rot);
  slrotate_z->setValue(prev_z_rot);

  slmove_x->setValue(prev_x_move);
  slmove_y->setValue(prev_y_move);
  slmove_z->setValue(prev_z_move);

  main_layout->setSpacing(25);

  scale_layout->addSpacerItem(left_spacer);
  scale_layout->addWidget(scale_sub_but);
  scale_layout->addWidget(scale_add_but);
  scale_layout->addSpacerItem(right_spacer);
  scale_group->setLayout(scale_layout);

  color_group->setLayout(color_layout);

  rot_x_layout->addWidget(label_x_sub);
  rot_x_layout->addWidget(slrotate_x, 1);
  rot_x_layout->addWidget(label_x_add);
  rotate_layout->addLayout(rot_x_layout);
  rot_y_layout->addWidget(label_y_sub);
  rot_y_layout->addWidget(slrotate_y, 1);
  rot_y_layout->addWidget(label_y_add);
  rotate_layout->addLayout(rot_y_layout);
  rot_z_layout->addWidget(label_z_sub);
  rot_z_layout->addWidget(slrotate_z, 1);
  rot_z_layout->addWidget(label_z_add);
  rotate_layout->addLayout(rot_z_layout);
  rotate_group->setLayout(rotate_layout);

  move_x_layout->addWidget(label_x_sub1);
  move_x_layout->addWidget(slmove_x, 1);
  move_x_layout->addWidget(label_x_add1);
  move_layout->addLayout(move_x_layout);
  move_y_layout->addWidget(label_y_sub1);
  move_y_layout->addWidget(slmove_y, 1);
  move_y_layout->addWidget(label_y_add1);
  move_layout->addLayout(move_y_layout);
  move_z_layout->addWidget(label_z_sub1);
  move_z_layout->addWidget(slmove_z, 1);
  move_z_layout->addWidget(label_z_add1);
  move_layout->addLayout(move_z_layout);
  move_group->setLayout(move_layout);

  rotate_group->setVisible(false);
  move_group->setVisible(false);
  color_group->setVisible(false);

  scale_group->setFixedHeight(70);

  main_layout->addWidget(my_gl);
  main_layout->addWidget(but);
  main_layout->addWidget(save_but);
  main_layout->addWidget(record_but);
  main_layout->addWidget(projection_but);
  main_layout->addWidget(reset_defaults_but);
  main_layout->addWidget(scale_group);
  main_layout->addWidget(show_color_but);
  main_layout->addWidget(color_group);
  main_layout->addWidget(show_rotate_but);
  main_layout->addWidget(rotate_group);
  main_layout->addWidget(show_move_but);
  main_layout->addWidget(move_group);
  setCentralWidget(central_widget);

  adjustSize();

  connect(but, &QPushButton::clicked, this, &MainWindow::openFileDialog);
  connect(background_color_but, &QPushButton::clicked, this,
          &::MainWindow::SetWidgetBackground);
  connect(projection_but, &QPushButton::clicked, this,
          &MainWindow::ToggleProjection);
  connect(scale_sub_but, &QPushButton::pressed, this, &MainWindow::ScaleSub);
  connect(scale_add_but, &QPushButton::pressed, this, &MainWindow::ScaleAdd);

  connect(slrotate_x, &QScrollBar::valueChanged, this,
          &MainWindow::xRotateChanged);
  connect(slrotate_y, &QScrollBar::valueChanged, this,
          &MainWindow::yRotateChanged);
  connect(slrotate_z, &QScrollBar::valueChanged, this,
          &MainWindow::zRotateChanged);

  connect(slmove_x, &QScrollBar::valueChanged, this, &MainWindow::xMoveChanged);
  connect(slmove_y, &QScrollBar::valueChanged, this, &MainWindow::yMoveChanged);
  connect(slmove_z, &QScrollBar::valueChanged, this, &MainWindow::zMoveChanged);
  connect(edgeColorButton, &QPushButton::clicked, this,
          &MainWindow::SetEdgeColor);
  connect(vertexColorButton, &QPushButton::clicked, this,
          &MainWindow::SetVertexColor);
  connect(edgeWidthSlider, &QSlider::valueChanged, this,
          &MainWindow::SetEdgeWidth);
  connect(vertexSizeSlider, &QSlider::valueChanged, this,
          &MainWindow::SetVertexSize);
  connect(edgeTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &MainWindow::SetEdgeType);
  connect(vertexDisplayTypeComboBox,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::SetVertexDisplayType);
  connect(save_but, &QPushButton::clicked, this,
          &MainWindow::SaveRenderedImage);
  connect(record_but, &QPushButton::clicked, this, &MainWindow::StartRecording);
  connect(timer, &QTimer::timeout, this, &MainWindow::CaptureFrame);

  // Подключить сигналы кнопок для переключения видимости групп
  connect(show_rotate_but, &QPushButton::clicked, this,
          &MainWindow::ToggleRotateGroup);
  connect(show_move_but, &QPushButton::clicked, this,
          &MainWindow::ToggleMoveGroup);
  connect(show_color_but, &QPushButton::clicked, this,
          &MainWindow::ToggleColorGroup);

  connect(reset_defaults_but, &QPushButton::clicked, this,
          &MainWindow::ResetToDefaults);
}

MainWindow::~MainWindow() { SaveSettings(); }

void MainWindow::openFileDialog() {
  file_name = QFileDialog::getOpenFileName(
      this, tr("Open File"), QDir::homePath(), tr("All Files (*.obj)"));
  if (!file_name.isEmpty()) {
    my_gl->ReinitializeOpenGL(file_name);
    info_label->setText(my_gl->GetInfoText());
    main_layout->insertWidget(2, info_label);
  }
}

void MainWindow::SetWidgetBackground() {
  QColor color = QColorDialog::getColor(my_gl->GetBackgroundColor(), nullptr,
                                        "Выберите цвет фона");
  if (color.isValid()) {
    my_gl->SetBackgroundColor(color);
  }
}

void MainWindow::UpdateProjectionButtonText() {
  if (my_gl->GetProjectionType() == glView::ProjectionType::Perspective) {
    projection_but->setText("Perspective Projection");
  } else {
    projection_but->setText("Orthographic Projection");
  }
}

void MainWindow::ToggleProjection() {
  if (my_gl->GetProjectionType() == glView::ProjectionType::Perspective) {
    my_gl->SetProjectionType(glView::ProjectionType::Orthographic);
  } else {
    my_gl->SetProjectionType(glView::ProjectionType::Perspective);
  }
  UpdateProjectionButtonText();
}

void MainWindow::xRotateChanged(int val) {
  int delta = val - prev_x_rot;
  prev_x_rot = val;
  my_gl->RotateVertices(delta * 4, glView::eCoord::X);
}

void MainWindow::yRotateChanged(int val) {
  int delta = val - prev_y_rot;
  prev_y_rot = val;
  my_gl->RotateVertices(delta * 4, glView::eCoord::Y);
}

void MainWindow::zRotateChanged(int val) {
  int delta = val - prev_z_rot;
  prev_z_rot = val;
  my_gl->RotateVertices(delta * 4, glView::eCoord::Z);
}

void MainWindow::xMoveChanged(int val) {
  int delta = val - prev_x_move;
  prev_x_move = val;
  my_gl->MoveVertices(delta, glView::eCoord::X);
}

void MainWindow::yMoveChanged(int val) {
  int delta = val - prev_y_move;
  prev_y_move = val;
  my_gl->MoveVertices(delta, glView::eCoord::Y);
}

void MainWindow::zMoveChanged(int val) {
  int delta = val - prev_z_move;
  prev_z_move = val;
  my_gl->MoveVertices(delta, glView::eCoord::Z);
}

void MainWindow::ScaleSub() { my_gl->ScaleVertices(-scale_val); }

void MainWindow::ScaleAdd() { my_gl->ScaleVertices(scale_val); }

void MainWindow::SetEdgeColor() {
  QColor color =
      QColorDialog::getColor(my_gl->GetEdgeColor(), this, "Select Edge Color");
  if (color.isValid()) {
    my_gl->SetEdgeColor(color);
  }
}

void MainWindow::SetVertexColor() {
  QColor color = QColorDialog::getColor(my_gl->GetVertexColor(), this,
                                        "Select Vertex Color");
  if (color.isValid()) {
    my_gl->SetVertexColor(color);
  }
}

void MainWindow::SetEdgeWidth(int width) { my_gl->SetEdgeWidth(width); }

void MainWindow::SetVertexSize(int size) { my_gl->SetVertexSize(size); }

void MainWindow::SetEdgeType(int type) { my_gl->SetEdgeType(type); }

void MainWindow::SetVertexDisplayType(int type) {
  my_gl->SetVertexDisplayType(type);
}

void MainWindow::SaveSettings() {
  settings.setValue("windowSize", size());
  settings.setValue("windowPosition", pos());
  settings.setValue("edgeColor", my_gl->GetEdgeColor());
  settings.setValue("vertexColor", my_gl->GetVertexColor());
  settings.setValue("backgroundColor", my_gl->GetBackgroundColor());
  settings.setValue("edgeWidth", edgeWidthSlider->value());
  settings.setValue("vertexSize", vertexSizeSlider->value());
  settings.setValue("edgeType", edgeTypeComboBox->currentIndex());
  settings.setValue("vertexDisplayType",
                    vertexDisplayTypeComboBox->currentIndex());
  settings.setValue("projectionType",
                    static_cast<int>(my_gl->GetProjectionType()));

  settings.setValue("modelFileName", file_name);
  settings.setValue("prev_x_rot", prev_x_rot);
  settings.setValue("prev_y_rot", prev_y_rot);
  settings.setValue("prev_z_rot", prev_z_rot);
  settings.setValue("prev_x_move", prev_x_move);
  settings.setValue("prev_y_move", prev_y_move);
  settings.setValue("prev_z_move", prev_z_move);
  settings.setValue("scale_val", scale_val);
}

void MainWindow::LoadSettings() {
  resize(settings.value("windowSize", QSize(fixed_size, 480)).toSize());
  move(settings.value("windowPosition", QPoint(200, 200)).toPoint());
  my_gl->SetEdgeColor(
      settings.value("edgeColor", QColor(Qt::black)).value<QColor>());
  my_gl->SetVertexColor(
      settings.value("vertexColor", QColor(Qt::black)).value<QColor>());
  my_gl->SetBackgroundColor(
      settings.value("backgroundColor", QColor(Qt::white)).value<QColor>());
  edgeWidthSlider->setValue(settings.value("edgeWidth", 1).toInt());
  vertexSizeSlider->setValue(settings.value("vertexSize", 1).toInt());
  edgeTypeComboBox->setCurrentIndex(settings.value("edgeType", 0).toInt());
  vertexDisplayTypeComboBox->setCurrentIndex(
      settings.value("vertexDisplayType", 0).toInt());

  int projectionType =
      settings
          .value("projectionType",
                 static_cast<int>(glView::ProjectionType::Perspective))
          .toInt();
  my_gl->SetProjectionType(static_cast<glView::ProjectionType>(projectionType));
  UpdateProjectionButtonText();

  prev_x_rot = settings.value("prev_x_rot", 50).toInt();
  prev_y_rot = settings.value("prev_y_rot", 50).toInt();
  prev_z_rot = settings.value("prev_z_rot", 50).toInt();
  prev_x_move = settings.value("prev_x_move", 50).toInt();
  prev_y_move = settings.value("prev_y_move", 50).toInt();
  prev_z_move = settings.value("prev_z_move", 50).toInt();
  scale_val = settings.value("scale_val", 10).toInt();

  slrotate_x->setValue(prev_x_rot);
  slrotate_y->setValue(prev_y_rot);
  slrotate_z->setValue(prev_z_rot);
  slmove_x->setValue(prev_x_move);
  slmove_y->setValue(prev_y_move);
  slmove_z->setValue(prev_z_move);
}

void MainWindow::SaveRenderedImage() {
  QImage image(my_gl->size(), QImage::Format_RGB32);
  QPainter painter(&image);

  my_gl->render(&painter);

  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save Image"), "",
      tr("BMP Files (*.bmp);;JPEG Files (*.jpeg *.jpg)"));

  if (!fileName.isEmpty()) {
    image.save(fileName);
  }
}

void MainWindow::StartRecording() {
  gif = new QGifImage;
  gif->setDefaultDelay(100);
  frameCount = 0;
  timer->start(100);
}

void MainWindow::CaptureFrame() {
  QImage image(QSize(640, 480), QImage::Format_RGB32);
  QPainter painter(&image);
  my_gl->render(&painter);

  gif->addFrame(image);

  frameCount++;

  if (frameCount >= 50) {
    timer->stop();
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save Screencast"), "", tr("GIF Files (*.gif)"));
    if (!fileName.isEmpty()) {
      gif->save(fileName);
    }
    delete gif;
    gif = nullptr;
  }
}

void MainWindow::ToggleColorGroup() {
  color_group->setVisible(!color_group->isVisible());
  if (!color_group->isVisible()) {
    setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    resize(originalSize);
    setFixedWidth(fixed_size);
  }
}

void MainWindow::ToggleRotateGroup() {
  rotate_group->setVisible(!rotate_group->isVisible());
  if (!rotate_group->isVisible()) {
    setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    resize(originalSize);
    setFixedWidth(fixed_size);
  }
}

void MainWindow::ToggleMoveGroup() {
  move_group->setVisible(!move_group->isVisible());
  if (!move_group->isVisible()) {
    setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    resize(originalSize);
    setFixedWidth(fixed_size);
  }
}

void MainWindow::ResetToDefaults() {
  my_gl->SetEdgeColor(QColor(Qt::blue));
  my_gl->SetVertexColor(QColor(Qt::black));
  my_gl->SetBackgroundColor(QColor(Qt::white));

  edgeWidthSlider->setValue(1);
  vertexSizeSlider->setValue(1);

  edgeTypeComboBox->setCurrentIndex(0);
  vertexDisplayTypeComboBox->setCurrentIndex(0);

  my_gl->SetProjectionType(glView::ProjectionType::Perspective);
  UpdateProjectionButtonText();

  slrotate_x->setValue(50);
  slrotate_y->setValue(50);
  slrotate_z->setValue(50);
  slmove_x->setValue(50);
  slmove_y->setValue(50);
  slmove_z->setValue(50);

  my_gl->ReinitializeOpenGL(file_name);
  my_gl->update();
}

void MainWindow::showEvent(QShowEvent *event) {
  QMainWindow::showEvent(event);
  LoadSettings();
  adjustSize();
}
