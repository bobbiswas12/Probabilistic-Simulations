
/* Monte Carlo implementation for finding out the value of Pi */
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
#include <stdlib.h>
#include <time.h>

#define screen_width 800
#define screen_height 800
#define fps 200
#define radius 100
#define no_of_points 10000

float random_value(float value){

  return (((float) rand()/(float) RAND_MAX)*(float) value);
}

void draw_axis(float width,float height){

  Vector2 y_top = (Vector2) {width/2, 0};
  Vector2 y_bottom = (Vector2) {width/2,height};
  
  Vector2 x_left = (Vector2) {0, height/2};
  Vector2 x_right = (Vector2) {width,height/2};

  DrawLineV(y_top,y_bottom,WHITE);
  DrawLineV(x_left,x_right,WHITE);

}

Vector2 random_position(float width,float height,float Radius){

  float x = (width)/2 + random_value(Radius);
  float y = (height/2 - Radius) + random_value(Radius);

  Vector2 position = (Vector2) {x,y};
  return position;

}

float distance(Vector2 p1, Vector2 p2){

  float x = (p1.x - p2.x)*(p1.x - p2.x);
  float y = (p1.y - p2.y)*(p1.y - p2.y);

  return x + y;
}
int main(){

  InitWindow(screen_width,screen_height,"pi");
  SetTargetFPS(fps);
  srand(time(0));

  Vector2 points[no_of_points] = {0};

  Vector2 origin = (Vector2) {screen_width/2,screen_height/2};
  
  for (int i = 0; i < no_of_points; i++){

    points[i] = random_position(screen_width,screen_height,radius);

  }
  int i = 0;
  float pi = 0;
  while (!WindowShouldClose()){

    BeginDrawing();
    ClearBackground(BLACK);

    DrawFPS(10,10);
    draw_axis(screen_width,screen_height);
    Vector2 position = (Vector2) {screen_width/2,screen_height/2};
    DrawCircleLinesV(position,radius,WHITE);

    if (i < no_of_points){
      i++;
    }
    int good_points = 0;
    for (int j = 0; j < i; j++){

      if (distance(points[j],origin) <= radius*radius && (points[j].x-origin.x) >=0 && (origin.y - points[j].y) >= 0){
	/* DrawCircleV(points[j],3,BLUE); */
	DrawPixelV(points[j],BLUE);
	good_points++;
      }
      else{
	/* DrawCircleV(points[j],3,RED); */
	DrawPixelV(points[j],RED);
      }
    }
    pi = ((float)good_points)/(float)i;
    DrawText(TextFormat("Pi: %f", 4 * pi),10,50,30,GREEN);
    DrawText(TextFormat("Points: %d", i),10,80,30,GREEN);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
