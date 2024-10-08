#ifndef C_COMMANDS_H_
#define C_COMMANDS_H_

#include "controller.h"

typedef struct Command {
  void (*execute)(struct Command*, int);
} Command;

typedef struct MoveCommand {
  Command base;
  s21::Controller* controller;
  char axis;
} MoveCommand;

void MoveCommand_execute(MoveCommand* self, int delta) {
  self->controller->Move(delta, self->axis);
}

typedef struct RotateCommand {
  Command base;
  s21::Controller* controller;
  char axis;
} RotateCommand;

void RotateCommand_execute(RotateCommand* self, int angle) {
  self->controller->Rotate(angle, self->axis);
}

typedef struct ScaleCommand {
  Command base;
  s21::Controller* controller;
} ScaleCommand;

void ScaleCommand_execute(ScaleCommand* self, int scale) {
  self->controller->Scale(scale);
}

#endif  // C_COMMANDS_H_