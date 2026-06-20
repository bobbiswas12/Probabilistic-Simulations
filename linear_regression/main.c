
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

#include <stdio.h>
#include "raylib.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define screen_height 768
#define screen_width 1366
#define origin (Vector2) {screen_width/2.0, screen_height/2.0}
#define x_partitions 40
#define y_partitions 40
#define fps 30
#define eps 1e-5
#define learning_rate 1e-2
#define data_size 49

float random_value(float value){

  return (((float) rand()/(float) RAND_MAX)*(float) value);
}


typedef struct {

  Vector2 points[data_size];
} data;

typedef struct {

  float a;
  float b;
} model;


void draw_graph(float screen_h, float screen_w){

  Vector2 y_pos = (Vector2) {origin.x,0};
  Vector2 y_neg = (Vector2) {origin.x,screen_h};
  Vector2 x_pos = (Vector2) {screen_w,origin.y};
  Vector2 x_neg = (Vector2) {0,origin.y};

  DrawLineV(y_pos, y_neg, WHITE);
  DrawLineV(x_pos, x_neg, WHITE);

  /* float x_partition_size = (float)(screen_width - origin.x) / x_partitions; */
  
  /* for (int i = 1; i <= x_partitions; i++) { */

  /*   Vector2 x_pos = (Vector2){origin.x + i * x_partition_size, 0}; */
  /*   Vector2 x_neg = (Vector2){origin.x + i * x_partition_size, screen_height}; */

  /*   Vector2 _x_pos = (Vector2){origin.x - i * x_partition_size, 0}; */
  /*   Vector2 _x_neg = (Vector2){origin.x - i * x_partition_size, screen_height}; */


  /*   DrawLineV(x_pos, x_neg, DARKGRAY); */
  /*   DrawLineV(_x_pos, _x_neg, DARKGRAY); */
  /* } */

  /* float y_partition_size = (float)(screen_height - origin.y) / y_partitions; */
  
  /* for (int i = 1; i <= y_partitions; i++) { */

  /*   Vector2 y_pos = (Vector2){0,origin.y + i * y_partition_size}; */
  /*   Vector2 y_neg = (Vector2){screen_width,origin.y + i * y_partition_size}; */

  /*   Vector2 _y_pos = (Vector2){0,origin.y - i * y_partition_size}; */
  /*   Vector2 _y_neg = (Vector2){screen_width,origin.y - i * y_partition_size}; */


  /*   DrawLineV(y_pos, y_neg, DARKGRAY); */
  /*   DrawLineV(_y_pos, _y_neg, DARKGRAY); */
  /* } */
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

void draw_point(Vector2 point) { DrawCircleV(point, 3.5, BLUE); }

void draw_data(data dat) {

  for (int i = 0; i < data_size; i++) {
    
    Vector2 data_point = dat.points[i];
    transform_point(&data_point);
    draw_point(data_point);
  }
}

float cost_function(data dat,model parameters) {

  float cf = 0;

  for (int i = 0; i < data_size; i++) {
    float y_real = dat.points[i].y;
    float y_predicted = (float) parameters.a * dat.points[i].x + parameters.b;

    cf += (y_real - y_predicted) * (y_real - y_predicted);
  }

  return 0.5 * cf / data_size;
}

void update_model(model *parameters, data dat) {

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
  DrawLineV(above_extreme_point,lower_extreme_point,RED);
}

void highlight_points(data dat, model parameters) {

  for (int i = 0; i < data_size; i++) {

    float distance =
        fabsf(parameters.a * dat.points[i].x - dat.points[i].y + parameters.b) /
        sqrtf(parameters.a * parameters.a + 1);

    if (distance < 0.5) {
      Vector2 data_point = dat.points[i];
      transform_point(&data_point);
      DrawCircleV(data_point,5,GREEN);
    }
  }
}
      
int main(){

  srand(time(0));
  InitWindow(screen_width, screen_height, "SLR");
  SetTargetFPS(fps);

  data dat = {0};

  for (int i = 0; i < data_size; i++) {
    
    float x, y;
    
    printf("Enter Point %d: ", i);
    scanf("%f %f",&x,&y);

    x = random_value(40) - 20;
    y = x - random_value(10) + 5;
    
    dat.points[i] = (Vector2){x, y};
  }
  model parameters;
  parameters.a = -1.1;
  parameters.b = -80;
  
  
  while (!WindowShouldClose()) {

    BeginDrawing();
    
    ClearBackground(BLACK);

    draw_graph(screen_height, screen_width);
    draw_data(dat);
    update_model(&parameters, dat);
    draw_line(parameters);
    highlight_points(dat,parameters);
    DrawText(TextFormat("Cost Function: %f", cost_function(dat, parameters)),
             10, 30, 20, YELLOW);
    DrawText(TextFormat("Alpha: %f", parameters.a), 10, 50, 20, YELLOW);
    DrawText(TextFormat("Beta: %f", parameters.b), 10, 70, 20, YELLOW);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
