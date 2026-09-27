/* Boltzmann Distribution Visualized using the example of a lattice containing 400 quanta. */
/* Copyright (C) 2026  Tanmay Rai */

/* Boltzmann Distribution Visualized using the example of a lattice containing 400 quanta. */
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
#include <time.h>
#include <stdlib.h>
#include "raylib.h"
#include <unistd.h>

#define screen_width 1366
#define screen_height 768
#define fps 240
#define sites 25
#define x_end screen_width / 2
#define y_end screen_height
#define outer_space 25
#define quanta 30
#define initial_quantum_value 1

float random_value(float value) {

  return (((float)rand() / (float)RAND_MAX) * (float)value);
}

void draw_lattice() {
  float lattice_width = (float) x_end - 2 * outer_space;
  float lattice_height = y_end - 2 * outer_space;

  float cell_width = lattice_width / sites;
  float cell_height = lattice_height / sites;

  for (int i = 0; i <= sites; i++) {
      float x = outer_space + i * cell_width;
      float y = outer_space + i * cell_height;

      DrawLine(outer_space, y, outer_space + lattice_width, y, WHITE);
      DrawLine(x, outer_space, x, outer_space + lattice_height, WHITE);
  }
}

typedef struct {
  
  Vector2 pos;
  int energy;

} cell;

typedef struct {

  cell cells[sites][sites];
} lattice;

void write_energies_in_cell(cell *cellular) {

  float lattice_width = (float) x_end - 2 * outer_space;
  float lattice_height = y_end - 2 * outer_space;

  float cell_width = lattice_width / sites;
  float cell_height = lattice_height / sites;

  int text;
  text = cellular->energy;
  float text_width = MeasureText(TextFormat("%d",text),10);
  float x = cellular->pos.x + (cell_width - text_width) / 2;

  float y = cellular->pos.y + (cell_height - 10) / 2;

  DrawText(TextFormat("%d", text), x, y, 10, WHITE);
}

void write_energy_in_lattice(lattice *model) {

  for (int i = 0; i < sites; i++) {
    for (int j = 0; j < sites; j++) {

      write_energies_in_cell(&model->cells[i][j]);
    }
  }
}

void initialize_model(lattice *model) {

  
  float lattice_width = (float)x_end - 2 * outer_space;
  float lattice_height = (float)y_end - 2 * outer_space;

  float cell_width = lattice_width / sites;
  float cell_height = lattice_height / sites;

  for (int i = 0; i < sites; i++) {
    for (int j = 0; j < sites; j++) {

      model->cells[i][j].pos.x = outer_space + j * cell_width;
      model->cells[i][j].pos.y = outer_space + i * cell_height;

      model->cells[i][j].energy = initial_quantum_value;
    }
  }
}

void update_cell(cell *cellular,int new_energy) {

  if (cellular->energy == 0 && new_energy < 0) {
    ;
  }
  else{
    cellular->energy = cellular->energy + new_energy;
  }
}

void update_lattice(lattice *model) {

  int i = random_value(sites);
  int j = random_value(sites);

  if (model->cells[i][j].energy > 0) {
    
    update_cell(&model->cells[i][j], -1);

    int i_new = random_value(sites);
    int j_new = random_value(sites);

    if (i_new == i && j_new == j) {
      ;
    }
    else{
      update_cell(&model->cells[i_new][j_new], 1);
    }
  }
}

int count_quanta(int _quanta, lattice *model) {

  int count = 0;
  for (int i = 0; i < sites; i++) {
    for (int j = 0; j < sites; j++) {
      if (model->cells[i][j].energy == _quanta) {
        count++;
      }
    }
  }

  return count;
}

void draw_histogram(lattice *model) {

  float lattice_width = (float) x_end - 2 * outer_space;
  float lattice_height = y_end - 2 * outer_space;

  DrawLine(x_end + outer_space, outer_space + lattice_height,
           2 * x_end - outer_space, outer_space + lattice_height, WHITE);

  DrawLine(x_end + outer_space, outer_space + lattice_height,
           x_end + outer_space, outer_space, WHITE);

  for (int i = 0; i < quanta; i++) {

    int count = count_quanta(i, model);
    float hist_width = (x_end - 2 * outer_space) / quanta;
    float hist_height = (lattice_height * count/ (sites*sites)); 
    DrawRectangle(x_end + outer_space + i*hist_width,outer_space + lattice_height - hist_height, hist_width,  hist_height,SKYBLUE);
  }
    
}
 

int main() {

  InitWindow(screen_width, screen_height, "Boltzmann - d");
  SetTargetFPS(fps);

  lattice model;
  initialize_model(&model);

  while (!WindowShouldClose()) {

    BeginDrawing();

    ClearBackground(BLACK);

    draw_lattice();
    draw_histogram(&model);
    write_energy_in_lattice(&model);
    update_lattice(&model);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
