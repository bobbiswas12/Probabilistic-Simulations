#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "raylib.h"
#include <unistd.h>

#define screen_width 1366
#define screen_height 768
#define fps 5
#define grid_x 30
#define grid_y 30
#define points_y (float) screen_height / grid_y
#define points_x (float) (screen_width - 120) / grid_x

float random_value(float value){

  return (((float) rand()/(float) RAND_MAX)*(float) value);
}

void draw_graph(float screenw, float screenh) {

  Vector2 y_pos = (Vector2){100, 0};
  Vector2 y_neg = (Vector2){100, screenh};
  Vector2 x_neg = (Vector2){0, screenh / 2};
  Vector2 x_pos = (Vector2){screenw, screenh / 2};

  DrawLineV(y_pos, y_neg, WHITE);
  DrawLineV(x_pos, x_neg, WHITE);

  for (int i = -200; i <= grid_x; i++) {

    if (i!=0){

      Vector2 y_pos = (Vector2){100 + points_x * i, 0};
      Vector2 y_neg = (Vector2){100 + points_x * i, screenh};
      DrawLineV(y_pos, y_neg, DARKGRAY);
    }
  }

  for (int i = 1; i <= grid_y/2; i++) {

    if (i != 0) {
      
      Vector2 x_pos = (Vector2){0,screenh/2 - points_y * i };
      Vector2 x_neg = (Vector2){screenw, screenh/2 - points_y * i};
      DrawLineV(x_pos, x_neg, DARKGRAY);

      Vector2 x_pos_1 = (Vector2){0,screenh/2 + points_y * i };
      Vector2 x_neg_1 = (Vector2){screenw, screenh / 2 + points_y * i};
      DrawLineV(x_pos_1,x_neg_1,DARKGRAY);
    }
  }
}

void transform_point(Vector2 *point, Vector2 origin) {
  point->x = origin.x + point->x * points_x;
  point->y = origin.y - point->y * points_y;
}

void transform_point_back(Vector2 *point, Vector2 origin) {
  point->x = (point->x - origin.x)/ points_x;
  point->y = (origin.y - point->y)/ points_y;
}

void draw_point(Vector2 position_point,Vector2 origin) {

  /* Vector2 new_pos = (Vector2) {origin.x + points_x *
   * position_point.x,origin.y - points_y * position_point.y}; */
  transform_point(&position_point,origin);
  DrawCircleV(position_point, 5, DARKBLUE);
}

void path_from_to(Vector2 initial_point, Vector2 final_point) {

  /* DrawCircleV(final_point, 5, DARKBLUE); */
  DrawLineV(initial_point, final_point, GREEN);
}

void update(Vector2 *point, int dir) {

  if (dir == 1) {
    point->x = point->x + points_x;
    point->y = point->y + points_y;
  } else {
    point->x = point->x + points_x;
    point->y = point->y - points_y;
  }
}

void display_point(Vector2 point, Vector2 origin) {

  transform_point_back(&point,origin);
  Vector2 next = (Vector2){point.x - 1, point.y - 1};
  transform_point(&point, origin);
  transform_point(&next, origin);
  Vector2 mid =
    (Vector2){(next.x + origin.x) / 2, (next.y + origin.y) / 2};
  transform_point_back(&point,origin);
  DrawText(TextFormat("(%d,%d)", (int) point.x, (int) point.y), mid.x, mid.y, 20, WHITE);
  transform_point(&point,origin);
}


int main(){


  srand(time(0));
  InitWindow(screen_width,screen_height,"Random Walk");
  SetTargetFPS(fps);
  Vector2 origin = (Vector2){100, (float)screen_height / 2};

  Vector2 point_start;
  int steps;
  printf("Starting Point:");
  scanf("%f %f", &point_start.x, &point_start.y);
  printf("No. of Steps: ");
  scanf("%d",&steps);

  transform_point(&point_start, origin);
  int step_counter = 0;
  
  while (!WindowShouldClose()){

    BeginDrawing();
    ClearBackground(BLACK);
    DrawFPS(10, 10);
    display_point(origin,origin);

    draw_graph(screen_width, screen_height);
    draw_point(point_start, origin);

    int a = (int) random_value(2);
    Vector2 prev_point = point_start;
    update(&point_start, a);
    path_from_to(prev_point, point_start);
    display_point(point_start,origin);
    if (step_counter == steps) {
      DrawCircleV(point_start, 5, DARKBLUE);
      display_point(point_start,origin);
    }
    step_counter++;
    EndDrawing();

    if (step_counter > steps) {
      sleep(2);
      break;
    }
  }
  CloseWindow();
  return 0;
}
