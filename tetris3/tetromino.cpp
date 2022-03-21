/* tetromino.cpp --- 
 * 
 * Filename: tetromino.cpp
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
#include "tetromino.hpp"


// add two shapes in the tetromino
const tetromino_t tetromino_types[9] = {
  {"block",
   {{0,0,0,0}, 
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}}, 
   0,
   0,
   '%',
   {0,0,0}},
  {"tee",
   {{0,0,0,0}, 
    {1,1,1,0},
    {0,1,0,0},
    {0,0,0,0}}, 
   0,
   0,
   '%',
   {0,0,0}},
  {"zigzag_l",
   {{0,0,1,0}, 
    {0,1,1,0},
    {0,1,0,0},
    {0,0,0,0}}, 
   0,
   0,
   '%',
   {0,0,0}},
  {"zigzag_r",
   {{0,1,0,0}, 
    {0,1,1,0},
    {0,0,1,0},
    {0,0,0,0}}, 
   0,
   0,
   '%',
   {0,0,0}},
  {"lform_l",
   {{0,0,1,0}, 
    {0,0,1,0},
    {0,1,1,0},
    {0,0,0,0}}, 
   0,
   0,
   '%',
   {0,0,0}},
  {"lform_r",
   {{0,1,0,0}, 
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}}, 
   0,
   0,
   '%',
   {0,0,0}},
  {"pipe",
   {{0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0}}, 
   0,
   0,
   '%',
   {0,0,0}}
  ,
  {"2dots",
   {{0,0,0,0}, 
    {0,1,1,0},
    {0,0,0,0},
    {0,0,0,0}}, 
   0,
   0,
   '%',
   {0,0,0}},
  {"dot",
   {{0,0,0,0}, 
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0}}, 
   0,
   0,
   '%',
   {0,0,0}}
};

int check_collision (tetromino_t *tet) {
  /* Function to check if the tetromino is currenly overlapping
     anything other than a space.  Spaces are used to designate a site
     that is free of a character in ncurses.
     
     Uses ncurses function mvinchnstr to read back rows of characters
     that have been previously placed on the screen from the location
     where the tet is currently placed. 4 rows are read that represent
     the 4x4 area of a tetromino.

     If an overlap is detected, the function returns
     COLLIDE. Otherwise it returns SAFE.
  */
  int x,y;
  chtype row_buf[5];
  int num_chars;
  int i;
  for (y=0;y<+4;y++) {
    num_chars = mvinchnstr(tet->upper_left_y+y, tet->upper_left_x, row_buf, 4);
    for (x=0;x<4;x++) {
      if (tet->piece[x][y] && row_buf[x]!=' ') {
	return COLLIDE;
      }
    }
  }
  return SAFE;
}

int move_tet (tetromino_t *tet, int new_x, int new_y) {
  /* Moves the tetromino if it will not collide with another feature
     on the screen such as the well or a previously placed tets. 

     The old position of the tet is saved so that it can be returned
     to this position if there is a collision. The new position passed
     as an argument is applied to the tet. The tet is then checked for
     collision.

     If a collistion is detected, then the tet is returned back to
     it's original location and a status of MOVE_FAILED is
     returned. Otherwise a status if MOVE_OK is returned.
   */
  int old_x = tet->upper_left_x;
  int old_y = tet->upper_left_y;
  tet->upper_left_x = new_x;
  tet->upper_left_y = new_y;
  if (check_collision(tet) == COLLIDE) {
    tet->upper_left_x = old_x;
    tet->upper_left_y = old_y;
    return MOVE_FAILED;
  }  
  else {
    return MOVE_OK;
  }
}

int rotate_cw(tetromino_t *tet) {
  /* Rotates a tetromino clockwise. A copy of the tetromino is saved
   prior to executing the rotation. If a collision is detected as a
   result of the rotation, the original tet is restored and a status
   of MOVE_FAILED is returned. Otherwise MOVE_OK is returned.
  */
  char temp[4][4];
  int x,y;
  tetromino_t temp_tet;
  memcpy(&temp_tet,tet,sizeof(tetromino_t));
  for (x=0;x<4;x++) {
    for (y=0;y<4;y++) {
      temp[x][y] = tet->piece[y][3-x]; 
    }
  }
  memcpy(tet->piece,&temp,sizeof(tet->piece));
  if (check_collision(tet) == COLLIDE) {
    memcpy(tet,&temp_tet,sizeof(tetromino_t));
    return MOVE_FAILED;
  }
  else {
    return MOVE_OK;
  }
}

int rotate_ccw(tetromino_t *tet) {
  /* Rotates a tetromino counter clockwise. A copy of the tetromino is saved
   prior to executing the rotation. If a collision is detected as a
   result of the rotation, the original tet is restored and a status
   of MOVE_FAILED is returned. Otherwise MOVE_OK is returned.
  */
  char temp[4][4];
  int x,y;
  tetromino_t temp_tet;
  memcpy(&temp_tet,tet,sizeof(tetromino_t));
  for (x=0;x<4;x++) {
    for (y=0;y<4;y++) {
      temp[x][y] = tet->piece[3-y][x]; 
    }
  }
  memcpy(tet->piece,&temp,sizeof(tet->piece));
  if (check_collision(tet) == COLLIDE) {
    memcpy(tet,&temp_tet,sizeof(tetromino_t));
    return MOVE_FAILED;
  }
  else {
    return MOVE_OK;
  }
}


//create the tetromino with the random
tetromino_t *create_tetromino (int initial_x, int initial_y) {
  int type;
  tetromino_t *tet = (tetromino*) malloc(sizeof(tetromino_t));
  type = rand()%9;
  memcpy(tet, &tetromino_types[type], sizeof(tetromino_t));
  tet->upper_left_x = initial_x;
  tet->upper_left_y = initial_y;
  return(tet);
}

//display the tetromino
void display_tetromino(tetromino_t *tet) {
  int x,y;
  for (x=0;x<4;x++) {
    for (y=0;y<+4;y++) {
      if (tet->piece[x][y]) {
	mvprintw(tet->upper_left_y+y,tet->upper_left_x+x,"%c",tet->draw_char);
      }
    }
  }
}

//undisplay the tetromino
void undisplay_tetromino(tetromino_t *tet) {
  int x,y;
  for (x=0;x<4;x++) {
    for (y=0;y<+4;y++) {
      if (tet->piece[x][y]) {
	mvprintw(tet->upper_left_y+y,tet->upper_left_x+x," ",tet->draw_char);
      }
    }
  }
}

// print the tetromino. This is different from the display tetromino as this is displayed at the left side of the game and it is used to print the next tetromino in the game.
void print_tetromino(tetromino_t *tet, int row, int col, int color) {
  mvprintw(row - 1,col,"Next:",tet->draw_char); 
  int x,y;
  for (x=0;x<4;x++) {
    for (y=0;y<+4;y++) {
      if (tet->piece[x][y]) {
        attron(COLOR_PAIR(color));
	      mvprintw(row + y,col + x,"%c",tet->draw_char); 
        attroff(COLOR_PAIR(color));

      }
    }
  }
}

// unprint the tetromino. This is different from the undisplay tetromino.
void unprint_tetromino(tetromino_t *tet, int row, int col) {
  // mvprintw(row - 1,col,"     ",tet->draw_char); 
  int x,y;
  for (x=0;x<4;x++) {
    for (y=0;y<+4;y++) {
      if (tet->piece[x][y]) {
	mvprintw(row + y,col + x," ",tet->draw_char);
      }
    }
  }
}

//store the color on each x y value inside a 2d array
void store_color(tetromino_t *tet, int colorInfo [][WELL_HEIGHT], int colorPair, int wulx, int wuly){
  int x,y;
  for (x=0;x<4;x++) {
    for (y=0;y<+4;y++) {
      if (tet->piece[x][y]) {
        colorInfo[tet->upper_left_x+x-wulx][tet->upper_left_y+y-wuly] = colorPair;
      }
    }
  }
}

//store the base in the 2d array that store the information about whether it is on an obstrution
void store_base(tetromino_t *tet, int baseInfo [][WELL_HEIGHT], int wulx, int wuly) {
  int baseNumber = 0;
  int x,y;
  for (x=0;x<4;x++) {
    for (y=0;y<+4;y++) {
      if (tet->piece[x][y]) {
        int baseUnder = 3;
        baseUnder = baseInfo[tet->upper_left_x+x-wulx][tet->upper_left_y+y-wuly+1];

        if (baseUnder == 0 || baseUnder == 10) {baseNumber = 0;}
        if (baseUnder == 1 || baseUnder == 11) {baseNumber = 1;}
        if (baseUnder == 2 || baseUnder == 12) {baseNumber = 2;}
      }
    }
  }

  for (x=0;x<4;x++) {
    for (y=0;y<+4;y++) {
      if (tet->piece[x][y]) {
        baseInfo[tet->upper_left_x+x-wulx][tet->upper_left_y+y-wuly] = baseNumber ;
      }
    }
  }

}



int destroy_tetromino(tetromino_t *tet) {
  free(tet);
}

/* tetromino.cpp ends here */
