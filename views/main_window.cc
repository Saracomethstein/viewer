#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      my_gl(new glView()),
      but(new QPushButton("Upload model", this)),
      scale_sub_but(new QPushButton("-", this)),
      scale_add_but(new QPushButton("+", this)),
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
      central_widget(new QWidget(this)),
      main_layout(new QVBoxLayout(central_widget)),
      rotate_layout(new QVBoxLayout()),
      move_layout(new QVBoxLayout()),
      scale_group(new QGroupBox("Scale", this)),
      rotate_group(new QGroupBox("Rotate", this)),
      move_group(new QGroupBox("Move", this)),
      left_spacer(new QSpacerItem(20, 20, QSizePolicy::Expanding,
                                  QSizePolicy::Minimum)),
      right_spacer(new QSpacerItem(20, 20, QSizePolicy::Expanding,
                                   QSizePolicy::Minimum)),
      prev_x_rot(50),
      prev_y_rot(50),
      prev_z_rot(50),
      prev_x_move(50),
      prev_y_move(50),
      prev_z_move(50),
      scale_val(5) {
  setWindowTitle("3d_viewer_c");
  setFixedWidth(664);

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

  main_layout->addWidget(my_gl);
  main_layout->addWidget(but);
  main_layout->addWidget(scale_group);
  main_layout->addWidget(rotate_group);
  main_layout->addWidget(move_group);
  setCentralWidget(central_widget);

  connect(but, &QPushButton::clicked, this, &MainWindow::openFileDialog);
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
}

void MainWindow::openFileDialog() {
  QSize size = this->size();
  QString exfile_name = file_name;
  qDebug() << "Width:" << size.width() << "Height:" << size.height();
  file_name = QFileDialog::getOpenFileName(
      this, tr("Open File"), QDir::homePath(), tr("All Files (*.obj)"));
  if (!file_name.isEmpty()) {
    my_gl->ReinitializeOpenGL(file_name);
    info_label->setText(my_gl->GetInfoText());
    if (exfile_name == "") main_layout->insertWidget(2, info_label);
  }
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