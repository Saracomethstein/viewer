#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <qgifimage.h>

#include <QColorDialog>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QMainWindow>
#include <QScrollBar>
#include <QSettings>
#include <QShowEvent>
#include <QSlider>
#include <QSpacerItem>
#include <QTimer>
#include <QVBoxLayout>

#include "widget.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  static constexpr int fixed_size = 664;
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 protected:
  void showEvent(QShowEvent *event) override;
 private slots:
  void openFileDialog();
  void SetWidgetBackground();
  void ToggleProjection();
  void UpdateProjectionButtonText();
  void SaveRenderedImage();
  void xRotateChanged(int val);
  void yRotateChanged(int val);
  void zRotateChanged(int val);
  void xMoveChanged(int val);
  void yMoveChanged(int val);
  void zMoveChanged(int val);
  void ScaleSub();
  void ScaleAdd();
  void SetEdgeColor();
  void SetVertexColor();
  void SetEdgeWidth(int width);
  void SetVertexSize(int size);
  void SetEdgeType(int type);
  void SetVertexDisplayType(int type);
  void StartRecording();
  void CaptureFrame();
  void ToggleColorGroup();
  void ToggleMoveGroup();
  void ToggleRotateGroup();
  void ResetToDefaults();

 private:
  void SaveSettings();
  void LoadSettings();
  glView *my_gl;
  QPushButton *but, *scale_sub_but, *scale_add_but, *background_color_but,
      *projection_but, *edgeColorButton, *vertexColorButton, *save_but,
      *record_but, *show_color_but, *show_rotate_but, *show_move_but,
      *reset_defaults_but;
  QScrollBar *slscale, *slrotate_x, *slrotate_y, *slrotate_z, *slmove_x,
      *slmove_y, *slmove_z;
  QString file_name;
  QLabel *info_label, *label_x_sub, *label_y_sub, *label_z_sub, *label_x_add,
      *label_y_add, *label_z_add, *label_x_sub1, *label_y_sub1, *label_z_sub1,
      *label_x_add1, *label_y_add1, *label_z_add1;
  QHBoxLayout *rot_x_layout, *rot_y_layout, *rot_z_layout, *move_x_layout,
      *move_y_layout, *move_z_layout, *scale_layout;
  QGridLayout *color_layout;
  QWidget *central_widget;
  QVBoxLayout *main_layout, *rotate_layout, *move_layout;
  QGroupBox *scale_group, *rotate_group, *move_group, *color_group;
  QSpacerItem *left_spacer, *right_spacer;
  QSlider *edgeWidthSlider, *vertexSizeSlider;
  QComboBox *edgeTypeComboBox, *vertexDisplayTypeComboBox;
  QGifImage *gif;
  QTimer *timer;
  QSize originalSize;
  QSettings settings;
  int frameCount;
  int prev_x_rot;
  int prev_y_rot;
  int prev_z_rot;
  int prev_x_move;
  int prev_y_move;
  int prev_z_move;
  int scale_val;
};

#endif  // MAINWINDOW_H_
