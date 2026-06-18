#include <stdio.h>
#include "raylib.h"
#include <math.h>

#define screen_height 768
#define screen_width 1366
#define origin (Vector2) {screen_width/2.0, screen_height/2.0}
#define x_partitions 20
#define y_partitions 20
#define fps 15
#define eps 1e-3
#define learning_rate 1e-3

typedef struct {

  Vector2 points[10];
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

    Vector2 y_pos = (Vector2){0,origin.y + i * y_partition_size};
    Vector2 y_neg = (Vector2){screen_width,origin.y + i * y_partition_size};

    Vector2 _y_pos = (Vector2){0,origin.y - i * y_partition_size};
    Vector2 _y_neg = (Vector2){screen_width,origin.y - i * y_partition_size};


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

void draw_point(Vector2 point) { DrawCircleV(point, 2.5, BLUE); }

void draw_data(data dat) {

  for (int i = 0; i < 10; i++) {
    
    Vector2 data_point = dat.points[i];
    transform_point(&data_point);
    draw_point(data_point);
  }
}

float cost_function(data dat,model parameters) {

  float cf = 0;

  for (int i = 0; i < 10; i++) {
    float y_real = dat.points[i].y;
    float y_predicted = (float) parameters.a * dat.points[i].x + parameters.b;

    cf += (y_real - y_predicted) * (y_real - y_predicted);
  }

  return 0.5 * cf;
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

int main(){

  InitWindow(screen_width, screen_height, "SLR");
  
  SetTargetFPS(fps);

  data dat = {0};
  for (int i = 0; i < 10; i++) {
    dat.points[i] = (Vector2){i - 3, i + 3};
  }
  model parameters;
  parameters.a = 1;
  parameters.b = 0;
  
  
  while (!WindowShouldClose()) {

    BeginDrawing();
    
    ClearBackground(GetColor(0x181818AA));

    draw_graph(screen_height, screen_width);
    draw_data(dat);
    draw_line(parameters);
    update_model(&parameters, dat);
    draw_line(parameters);
    DrawText(TextFormat("Cost Function: %f",cost_function(dat,parameters)),10,30,20,YELLOW);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
