#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <QColorDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollBar>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollBar>

#include "widget.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
 private slots:
  void openFileDialog();
  void xRotateChanged(int val);
  void yRotateChanged(int val);
  void zRotateChanged(int val);
  void xMoveChanged(int val);
  void yMoveChanged(int val);
  void zMoveChanged(int val);
  void ScaleSub();
  void ScaleAdd();

 private:
  glView *my_gl;
  QPushButton *but, *scale_sub_but, *scale_add_but;
  QScrollBar *slscale, *slrotate_x, *slrotate_y, *slrotate_z, *slmove_x,
      *slmove_y, *slmove_z;
  QGroupBox *groupBox;
  QString file_name;
  QLabel *info_label, *label_x_sub, *label_y_sub, *label_z_sub, *label_x_add,
      *label_y_add, *label_z_add, *label_x_sub1, *label_y_sub1, *label_z_sub1,
      *label_x_add1, *label_y_add1, *label_z_add1;
  QHBoxLayout *rot_x_layout, *rot_y_layout, *rot_z_layout, *move_x_layout,
      *move_y_layout, *move_z_layout, *scale_layout;
  QWidget *central_widget;
  QVBoxLayout *main_layout, *rotate_layout, *move_layout;
  QGroupBox *scale_group, *rotate_group, *move_group;
  QSpacerItem *left_spacer, *right_spacer;
  int prev_x_rot;
  int prev_y_rot;
  int prev_z_rot;
  int prev_x_move;
  int prev_y_move;
  int prev_z_move;
  int scale_val;
};

#endif  // MAIN_WINDOW_H_
