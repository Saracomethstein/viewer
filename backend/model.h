#ifndef C_MODEL_H_
#define C_MODEL_H_

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

void rotate_model(obj_model* model, double angle, char axis);
void move_model(obj_model* model, int delta, char axis);
void scale_model(obj_model* model, int scale);

#endif  // C_MODEL_H_
