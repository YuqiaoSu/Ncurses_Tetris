/* well.hpp --- 
 * 
 * Filename: well.hpp
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

/* Copyright (c) 2016 IUB.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */
// Yuqiao Su, Zekai Shen 2022/3/12
/* Code: */
#ifndef WELL_H  // header guard
#define WELL_H

#include <map>
using namespace std;

#define WELL_WIDTH 25
#define WELL_HEIGHT 22


typedef struct well {
  int upper_left_x;
  int upper_left_y;
  int width;
  int height;
  char draw_char;
  char color[3];
} well_t;

// signals the position in well
typedef struct position_in_well{
  int x;
  int y;
  bool operator<(const position_in_well& t) const
    {
        return (this->x + this -> y < t.x + t.y);
  }
  bool operator==(const position_in_well &t) const {
		return x == t.x && y == t.y;
	}

} xyPos;

well_t *init_well(int, int, int, int);
void draw_well(well_t *);

well_t *changeWellSize(int upper_left_x, int upper_left_y, int width, int height, well_t *);

void undraw_well(well_t *);

void clean_well(well_t *);


int prune_well(well_t *w, int colorInfo [][WELL_HEIGHT], int baseInfo [][WELL_HEIGHT]);

void set_color(map<xyPos,int> & xycol);

void redraw_well(well_t *w, int colorInfo[WELL_WIDTH][WELL_HEIGHT]);

void redraw_well2(well_t *w, int baseInfo[WELL_WIDTH][WELL_HEIGHT]);

void initialize_well(int colorInfo[][WELL_HEIGHT]);

void initialize_base(int baseInfo[][WELL_HEIGHT]);

bool move_obstruction(int colorInfo[][WELL_HEIGHT], int on_struct[][WELL_HEIGHT], int xdirection);
#endif
/* well.hpp ends here */
