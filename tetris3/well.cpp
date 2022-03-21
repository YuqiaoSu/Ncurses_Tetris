/* well.cpp ---
 *
 * Filename: well.cpp
 * Description:
 * Author: Bryce
 * Maintainer: Adeel Bhutta
 * Created: Tue Sep  6 11:08:59 2016
 * Last-Updated: 01-10-2021
 *           By: Adeel Bhutta
 *     Update #: 0
 * Keywords:
 * Compatibility:
 *
 */

/* Commentary:
 *
 *
 *
 */

/* Change log:
 *
 *
 */

/* Copyright (c) 2016 IUB
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */
// Yuqiao Su, Zekai Shen 2022/3/12
/* Code: */

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <ncurses.h>
#include <map>
#include "well.hpp"
using namespace std;

#define WELL_WIDTH 25
#define WELL_HEIGHT 22



well_t *init_well(int upper_left_x, int upper_left_y, int width, int height)
{
  well_t *w;
  w = (well_t *)malloc(sizeof(well_t));
  w->upper_left_x = upper_left_x;
  w->upper_left_y = upper_left_y;
  w->width = width;
  w->height = height;
  w->draw_char = '#';
  w->color[0] = 0;
  w->color[1] = 0;
  w->color[2] = 0;
  return (w);
}



void draw_well(well_t *w)
{
  int row_counter, column_counter;
  // Draw left side of well
  for (column_counter = w->upper_left_y; column_counter <= (w->upper_left_y + w->height); column_counter++)
  {
    mvprintw(column_counter, w->upper_left_x, "%c", w->draw_char);
  }

  // Draw right side of well
  for (column_counter = w->upper_left_y; column_counter <= (w->upper_left_y + w->height); column_counter++)
  {
    mvprintw(column_counter, (w->upper_left_x + w->width), "%c", w->draw_char);
  }

  // Draw Bottom of well
  for (row_counter = w->upper_left_x; row_counter <= (w->upper_left_x + w->width); row_counter++)
  {
    mvprintw(w->upper_left_y + w->height, row_counter, "%c", w->draw_char);
  }
}


well_t *changeWellSize(int upper_left_x, int upper_left_y, int width, int height, well_t *w)
{

  w->upper_left_x = upper_left_x;
  w->upper_left_y = upper_left_y;
  if (width < 10)
    w->width = 10;
  else
    w->width = width;

  w->height = height;

  return (w);
}

void undraw_well(well_t *w)
{
  int row_counter, column_counter;
  // Undraw left side of well
  for (column_counter = w->upper_left_y; column_counter <= (w->upper_left_y + w->height); column_counter++)
  {
    mvprintw(column_counter, w->upper_left_x, " ", w->draw_char);
  }

  // Undraw right side of well
  for (column_counter = w->upper_left_y; column_counter <= (w->upper_left_y + w->height); column_counter++)
  {
    mvprintw(column_counter, (w->upper_left_x + w->width), " ", w->draw_char);
  }

  // Undraw Bottom of well
  for (row_counter = w->upper_left_x; row_counter <= (w->upper_left_x + w->width); row_counter++)
  {
    mvprintw(w->upper_left_y + w->height, row_counter, " ", w->draw_char);
  }
}

// clean all the things in the well.
void clean_well(well_t *w)
{
  undraw_well(w);
  int row_counter, column_counter;

  // go through all the element in the well and let it be ' '
  for (column_counter = w->upper_left_y; column_counter <= (w->upper_left_y + w->height); column_counter++)
  {
    for (row_counter = w->upper_left_x; row_counter <= (w->upper_left_x + w->width); row_counter++)
    {
      mvprintw(column_counter, row_counter, " ", w->draw_char);
    }
  }
  // then print the empty well.
  draw_well(w);
}

// delete a single row in the well

void delete_row(well_t *w, int row)
{
  int row_counter;
  for (row_counter = w->upper_left_x + 1; row_counter <= (w->upper_left_x + w->width) - 1; row_counter++)
  {
    chtype chstr;
    int num_chars;
    mvprintw(row, row_counter, " ", w->draw_char);
  }
}

//move the above row and drop down
void move_above(well_t *w, int row)
{
  int row_counter, column_counter, count;

  for (column_counter = row - 1; column_counter >= w->upper_left_y; column_counter--)
  {
    for (row_counter = w->upper_left_x + 1; row_counter <= (w->upper_left_x + w->width) - 1; row_counter++)
    {

      chtype chstr;
      int num_chars;
      num_chars = mvinchnstr(column_counter, row_counter, &chstr, 1);
      char ch = (chstr & A_CHARTEXT);

      if (chstr != ' ')
      {

        // xyPos above = {row_counter,column_counter + 1};
        // int color = xyColor.at(above);
        mvprintw(column_counter, row_counter, " ", w->draw_char);
        // attron(COLOR_PAIR(color));
        mvprintw(column_counter + 1, row_counter, "%c", '%');
        //   attroff(COLOR_PAIR(color));
      }
    }
  }
}

// return how many row is deleted
int prune_well(well_t *w, int colorInfo[][WELL_HEIGHT], int baseInfo[][WELL_HEIGHT])
{
  int row_counter, column_counter;
  int count = 0;

  for (column_counter = w->upper_left_y; column_counter <= (w->upper_left_y + w->height) - 1; column_counter++)
  {
    bool all_line = true;
    int num_chars;
    for (row_counter = w->upper_left_x + 1; row_counter <= (w->upper_left_x + w->width) - 1; row_counter++)
    {
      chtype chstr;
      num_chars = mvinchnstr(column_counter, row_counter, &chstr, 1);
      char ch = (chstr & A_CHARTEXT);
      all_line = all_line && (ch != ' ');
    }

    if (all_line)
    {
      count++;
      bool bases[4] = {false, false, false, false};

      for (int i = 1; i < WELL_WIDTH; i++)
      {
        if (colorInfo[i][column_counter - w->upper_left_y] < 6)
        {
          colorInfo[i][column_counter - w->upper_left_y] = 0;
        }
        if (baseInfo[i][column_counter - w->upper_left_y] == 0)
        {
          bases[0] = true;
        }
        if (baseInfo[i][column_counter - w->upper_left_y] == 1)
        {
          bases[1] = true;
        }
        if (baseInfo[i][column_counter - w->upper_left_y] == 2)
        {
          bases[2] = true;
        }

        baseInfo[i][column_counter - w->upper_left_y] = 0;
      }

      for (int i = 1; i < WELL_WIDTH; i++)
      {
        for (int j = column_counter; j > w->upper_left_y; j--)
        {
          if (colorInfo[i][j - w->upper_left_y - 1] < 6)
          {
            // remove obstruction1
            for (int i1 = 21; i1 < 25; i1++)
            {
              for (int j1 = 18; j1 < 22; j1++)
              {
                colorInfo[i1][j1] = 0;
                baseInfo[i1][j1] = 3;
              }
            }

            if (bases[baseInfo[i][j - w->upper_left_y]] && baseInfo[i][j - w->upper_left_y] == 2)
            {
              if (baseInfo[i][j - w->upper_left_y] != 12)
              {
                colorInfo[i][j - w->upper_left_y] = colorInfo[i][j - w->upper_left_y - 1];
                baseInfo[i][j - w->upper_left_y] = baseInfo[i][j - w->upper_left_y - 1];
              }
            }
            else if (bases[baseInfo[i][j - w->upper_left_y]])
            {
              if (!(baseInfo[i][j - w->upper_left_y] == 12 || baseInfo[i][j - w->upper_left_y] == 2))
              {
                colorInfo[i][j - w->upper_left_y] = colorInfo[i][j - w->upper_left_y - 1];
                baseInfo[i][j - w->upper_left_y] = baseInfo[i][j - w->upper_left_y - 1];
              }
            }

            // add obstruction1
            for (int i1 = 21; i1 < 25; i1++)
            {
              for (int j1 = 18; j1 < 22; j1++)
              {
                colorInfo[i1][j1] = 6;
                baseInfo[i1][j1] = 11;
              }
            }
          }
        }
      }
    }
  }

  return count;
}

//initialize the well
void initialize_well(int colorInfo[][WELL_HEIGHT])
{
  // clean the well
  for (int i = 0; i < WELL_WIDTH; i++)
  {
    for (int j = 0; j < WELL_HEIGHT; j++)
    {
      colorInfo[i][j] = 0;
    }
  }

  // add obstruction1
  for (int i = 21; i < 25; i++)
  {
    for (int j = 18; j < 22; j++)
    {
      colorInfo[i][j] = 6;
    }
  }

  // add obstruction2
  for (int i = 0; i < 5; i++)
  {
    for (int j = 10; j < 14; j++)
    {
      colorInfo[i][j] = 6;
    }
  }
}

//does this obstruction movable on current space
bool movable(int colorInfo[][WELL_HEIGHT], int baseInfo[][WELL_HEIGHT], int xdirection)
{
  for (int i = 0; i < WELL_WIDTH; i++)
  {
    for (int j = 0; j < 14; j++)
    {

      // the obstruction is movable?
      if (colorInfo[i][j] == 6)
      {
        if ((colorInfo[i + xdirection][j] != 6 && colorInfo[i + xdirection][j] != 0) || i + xdirection == WELL_WIDTH - 1 || i + xdirection == 0)
        {
          return false;
        }
      }
      if (colorInfo[i][j] != 6 && colorInfo[i][j] != 0)
      {
        if (baseInfo[i][j] == 2)
        {
          if (baseInfo[i + xdirection][j] != 2 && baseInfo[i + xdirection][j] != 3 || i + xdirection == WELL_WIDTH - 1 || i + xdirection == 0)
          {
            return false;
          }
        }
      }
    }
  }
  return true;
}

// move the obstruction with the given xdirection.
bool move_obstruction(int colorInfo[][WELL_HEIGHT], int baseInfo[][WELL_HEIGHT], int xdirection)
{
  // int colorBuffer[WELL_WIDTH][14] = {0};
  // int baseInfoBuffer[WELL_WIDTH][10] = {0};

  if (movable(colorInfo, baseInfo, xdirection))
  {
    if (xdirection > 0)
    {
      for (int i = WELL_WIDTH - 2; i >= 0; i--)
      {
        for (int j = 0; j < 14; j++)
        {
          if (baseInfo[i][j] == 2 || baseInfo[i][j] == 12)
          {
            baseInfo[i + xdirection][j] = baseInfo[i][j];
            baseInfo[i][j] = 3;
            colorInfo[i + xdirection][j] = colorInfo[i][j];
            colorInfo[i][j] = 0;
          }
        }
      }
    }
    if (xdirection < 0)
    {
      for (int i = 0; i < WELL_WIDTH - 1; i++)
      {
        for (int j = 0; j < 14; j++)
        {
          if (baseInfo[i][j] == 2 || baseInfo[i][j] == 12)
          {
            baseInfo[i + xdirection][j] = baseInfo[i][j];
            baseInfo[i][j] = 3;
            colorInfo[i + xdirection][j] = colorInfo[i][j];
            colorInfo[i][j] = 0;
          }
        }
      }
    }
    return true;
  }
  return false;
}

// redraw the well according to the color
void redraw_well(well_t *w, int colorInfo[WELL_WIDTH][WELL_HEIGHT])
{
  clean_well(w);
  int row_counter, column_counter;

  // go through all the element in the well and let it be ' '
  for (column_counter = w->upper_left_y; column_counter <= (w->upper_left_y + w->height); column_counter++)
  {
    for (row_counter = w->upper_left_x; row_counter <= (w->upper_left_x + w->width); row_counter++)
    {
      int x, y;
      y = column_counter - w->upper_left_y;
      x = row_counter - w->upper_left_x;
      int color = colorInfo[x][y];

      if (color >= 6)
      {
        if (color = 6)
        {
          mvprintw(column_counter, row_counter, "%c", '#');
        }
      }
      else if (color != 0)
      {
        attron(COLOR_PAIR(color));
        mvprintw(column_counter, row_counter, "%c", '%');
        attroff(COLOR_PAIR(color));
      }
    }
  }
  // then print the empty well.
  draw_well(w);
}

// test methods that redraw the well 
void redraw_well2(well_t *w, int baseInfo[WELL_WIDTH][WELL_HEIGHT])
{
  clean_well(w);
  int row_counter, column_counter;

  // go through all the element in the well and let it be ' '
  for (column_counter = w->upper_left_y; column_counter <= (w->upper_left_y + w->height); column_counter++)
  {
    for (row_counter = w->upper_left_x; row_counter <= (w->upper_left_x + w->width); row_counter++)
    {
      int x, y;
      y = column_counter - w->upper_left_y;
      x = row_counter - w->upper_left_x;
      int base = baseInfo[x][y];

      mvprintw(column_counter, row_counter, "%d", base);
    }
  }
  // then print the empty well.
  draw_well(w);
}

void initialize_base(int baseInfo[][WELL_HEIGHT])
{
  // set the base value of empty
  for (int i = 0; i < WELL_WIDTH; i++)
  {
    for (int j = 0; j < WELL_HEIGHT; j++)
    {
      baseInfo[i][j] = 3;
    }
  }

  // set base value of the ground
  for (int i = 0; i < WELL_WIDTH; i++)
  {
    baseInfo[i][WELL_HEIGHT - 1] = 10;
    //   baseInfo[i][WELL_HEIGHT-1] = 10;
  }

  // set the base value for obstruction 1
  for (int i = 21; i < 25; i++)
  {
    for (int j = 18; j < 22; j++)
    {
      baseInfo[i][j] = 11;
      // baseInfo[i][j] = 11;
    }
  }

  // set the base value for obstruction 2
  for (int i = 0; i < 5; i++)
  {
    for (int j = 10; j < 14; j++)
    {
      baseInfo[i][j] = 12;
      // baseInfo[i][j] = 12;
    }
  }
}


/* well.cpp ends here */
