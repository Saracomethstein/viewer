#ifndef CPP4_3DVIEWER_V_2_0_1_SRC_CONTROLLER_COMMANDS_H_
#define CPP4_3DVIEWER_V_2_0_1_SRC_CONTROLLER_COMMANDS_H_

#include "controller.h"

namespace s21 {

class Command {
 public:
  virtual ~Command() = default;
  virtual void execute(int) = 0;
};

class MoveCommand : public Command {
 public:
  MoveCommand(Controller& controller, char axis)
      : controller_(controller), axis_(axis) {}

  void execute(int delta) override { controller_.Move(delta, axis_); }

 private:
  Controller& controller_;
  char axis_;
};

class RotateCommand : public Command {
 public:
  RotateCommand(Controller& controller, char axis)
      : controller_(controller), axis_(axis) {}

  void execute(int angle) override { controller_.Rotate(angle, axis_); }

 private:
  Controller& controller_;
  char axis_;
};

class ScaleCommand : public Command {
 public:
  ScaleCommand(Controller& controller) : controller_(controller) {}

  void execute(int scale) override { controller_.Scale(scale); }

 private:
  Controller& controller_;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V_2_0_1_SRC_CONTROLLER_COMMANDS_H_
