/* tetromino.hpp --- 
 * 
 * Filename: tetromino.hpp
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

#include <map>
#include "well.hpp"

using namespace std;

#define WELL_WIDTH 25
#define WELL_HEIGHT 22

typedef struct tetromino {
  char type_str[20];
  char piece[4][4];             // 4x4 grid of characters, piece[x][y]
  int upper_left_x;
  int upper_left_y;
  char draw_char;
  char color[3];
} tetromino_t;

extern const tetromino_t tetromino_types[9];

enum {SAFE, COLLIDE}; // Return status for check_collision 
int check_collision (tetromino_t *);

enum {MOVE_OK, MOVE_FAILED}; // Return status for move
int move_tet (tetromino_t *, int, int);
int rotate_cw(tetromino_t *);
int rotate_ccw(tetromino_t *);
tetromino_t *create_tetromino (int, int);
int destroy_tetromino(tetromino_t *);
void print_tetromino(tetromino_t *tet, int row, int col,int color);
void unprint_tetromino(tetromino_t *tet, int row, int col);
void display_tetromino(tetromino_t *tet); // the display haven't added to the hpp file here so cannot be referrenced before added
void undisplay_tetromino(tetromino_t *tet);
void test_tetromino(void);

void store_color(tetromino_t *tet, int colorInfo [][WELL_HEIGHT], int colorPair, int wulx, int wuly); //remeber the color for moving tetris down
void store_base(tetromino_t *tet, int baseInfo [][WELL_HEIGHT], int wulx, int wuly);

/* tetromino.hpp ends here */
