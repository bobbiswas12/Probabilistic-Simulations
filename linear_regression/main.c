
/* Using Gradient Descent to Visualise Linear Regression Dynamically */
/* Copyright (C) 2026  Tanmay Rai */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define screen_height 768
#define screen_width 1366
#define origin (Vector2){screen_width / 2.0, screen_height / 2.0}
#define x_partitions 40
#define y_partitions 40
#define fps 120
#define eps 1e-5
#define learning_rate 1e-4
#define data_size 1000

float random_value(float value) {

  return (((float)rand() / (float)RAND_MAX) * (float)value);
}

typedef struct {

  Vector2 *points;
  int count;
  int size;
} dyn_array;

#define da_append(da, element)						\
  {									\
    if ((da).count < (da).size) {					\
      (da).points[(da).count] = element;				\
      (da).count++;							\
    } else {								\
      if ((da).size == 0) {						\
	(da).size = 2;							\
      }									\
      else {								\
	(da).size *= 2;							\
      }									\
      (da).points = realloc((da).points , (da).size * sizeof(*(da).points)); \
      (da).points[(da).count] = element;				\
      (da).count++;							\
    }									\
  }									\

typedef struct {

  Vector2 points[data_size];
} data;

typedef struct {

  float a;
  float b;
} model;

void draw_graph(float screen_h, float screen_w) {

  Vector2 y_pos = (Vector2){origin.x, 0};
  Vector2 y_neg = (Vector2){origin.x, screen_h};
  Vector2 x_pos = (Vector2){screen_w, origin.y};
  Vector2 x_neg = (Vector2){0, origin.y};

  DrawLineV(y_pos, y_neg, WHITE);
  DrawLineV(x_pos, x_neg, WHITE);

  float x_partition_size = (float)(screen_width - origin.x) / x_partitions;

  for (int i = 1; i <= x_partitions; i++) {

    Vector2 x_pos = (Vector2){origin.x + i * x_partition_size, 0};
    Vector2 x_neg = (Vector2){origin.x + i * x_partition_size, screen_height};

    Vector2 _x_pos = (Vector2){origin.x - i * x_partition_size, 0};
    Vector2 _x_neg = (Vector2){origin.x - i * x_partition_size, screen_height};

    DrawLineV(x_pos, x_neg, DARKGRAY);
    DrawLineV(_x_pos, _x_neg, DARKGRAY);
  }

  float y_partition_size = (float)(screen_height - origin.y) / y_partitions;

  for (int i = 1; i <= y_partitions; i++) {

    Vector2 y_pos = (Vector2){0, origin.y + i * y_partition_size};
    Vector2 y_neg = (Vector2){screen_width, origin.y + i * y_partition_size};

    Vector2 _y_pos = (Vector2){0, origin.y - i * y_partition_size};
    Vector2 _y_neg = (Vector2){screen_width, origin.y - i * y_partition_size};

    DrawLineV(y_pos, y_neg, DARKGRAY);
    DrawLineV(_y_pos, _y_neg, DARKGRAY);
  }
}

void transform_point(Vector2 *point) {

  float x_partition_size = (float)(screen_width - origin.x) / x_partitions;
  float y_partition_size = (float)(screen_height - origin.y) / y_partitions;

  float x_int_part = floorf(point->x);
  float x_frac_part = point->x - x_int_part;

  point->x = origin.x + (x_int_part + x_frac_part) * x_partition_size;

  float y_int_part = floorf(point->y);
  float y_frac_part = point->y - y_int_part;

  point->y = origin.y - (y_int_part + y_frac_part) * y_partition_size;
}

void transform_point_back(Vector2 *point) {

  float x_partition_size = (float)(screen_width - origin.x) / x_partitions;
  float y_partition_size = (float)(screen_height - origin.y) / y_partitions;

  point->x = (point->x - origin.x) / x_partition_size;
  point->y = (origin.y - point->y) / y_partition_size;
}
  

void draw_point(Vector2 point) { DrawCircleV(point, 3.5, BLUE); }

void draw_data(dyn_array dat) {

  for (int i = 0; i < dat.count; i++) {

    Vector2 data_point = dat.points[i];
    transform_point(&data_point);
    draw_point(data_point);
  }
}

float cost_function(dyn_array dat, model parameters) {

  float cf = 0;

  for (int i = 0; i < dat.count; i++) {
    float y_real = dat.points[i].y;
    float y_predicted = (float)parameters.a * dat.points[i].x + parameters.b;

    cf += (y_real - y_predicted) * (y_real - y_predicted);
  }

  return 0.5 * cf / dat.count;
}

void update_model(model *parameters, dyn_array dat) {

  model temp = {0};
  temp.a = parameters->a;
  temp.b = parameters->b;

  float cf = cost_function(dat, *parameters);
  temp.a += eps;
  parameters->a =
      parameters->a - learning_rate * (cost_function(dat, temp) - cf) / eps;
  temp.a -= eps;
  temp.b += eps;
  parameters->b =
      parameters->b - learning_rate * (cost_function(dat, temp) - cf) / eps;
}

void draw_line(model parameters) {

  /* y = ax + b; */

  Vector2 above_extreme_point = {0};
  Vector2 lower_extreme_point = {0};

  if (fabsf(parameters.a) > (float)screen_height / screen_width) {

    above_extreme_point = (Vector2){
        (float)(y_partitions - parameters.b) / parameters.a, y_partitions};
    transform_point(&above_extreme_point);

    lower_extreme_point = (Vector2){
        (float)(-y_partitions - parameters.b) / parameters.a, -y_partitions};
    transform_point(&lower_extreme_point);
  } else {

    above_extreme_point = (Vector2){
        x_partitions, (float)parameters.a * x_partitions + parameters.b};
    transform_point(&above_extreme_point);

    lower_extreme_point = (Vector2){
        -x_partitions, (float)parameters.a * (-x_partitions) + parameters.b};
    transform_point(&lower_extreme_point);
  }
  DrawLineV(above_extreme_point, lower_extreme_point, WHITE);
}

void highlight_points(dyn_array dat, model parameters) {

  for (int i = 0; i < dat.count; i++) {

    float distance =
        fabsf(parameters.a * dat.points[i].x - dat.points[i].y + parameters.b) /
        sqrtf(parameters.a * parameters.a + 1);

    if (distance < 0.1) {
      Vector2 data_point = dat.points[i];
      transform_point(&data_point);
      DrawCircleV(data_point, 5, GREEN);
    }
  }
}

void add_point(dyn_array *dat, Vector2 point_to_be_added) {

  da_append(*dat,point_to_be_added);
}

int main() {

  srand(time(0));
  InitWindow(screen_width, screen_height, "SLR");
  SetTargetFPS(fps);

  dyn_array dat = {0};
  for (int i = 0; i < 2; i++) {

    float x;

    x = i;

    Vector2 point = (Vector2){x,x};
    
    da_append(dat,point);
  }
  model parameters;
  parameters.a = 1;
  parameters.b = 0;

  while (!WindowShouldClose()) {

    BeginDrawing();

    ClearBackground(BLACK);

    draw_graph(screen_height, screen_width);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      Vector2 pos = GetMousePosition();
      transform_point_back(&pos);
      add_point(&dat, pos);
    }
    draw_data(dat);
    draw_line(parameters);
    update_model(&parameters, dat);
    highlight_points(dat, parameters);
    DrawText(TextFormat("Cost Function: %f", cost_function(dat, parameters)),
             10, 30, 20, YELLOW);
    DrawText(TextFormat("a: %f", parameters.a), 10, 50, 20, YELLOW);
    DrawText(TextFormat("b: %f", parameters.b), 10, 70, 20, YELLOW);
    DrawText(TextFormat("model: y = %fx + %f", parameters.a, parameters.b), 10,
             90, 20, YELLOW);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
