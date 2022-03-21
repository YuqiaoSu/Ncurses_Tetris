/* game.hpp --- 
 * 
 * Filename: game.hpp
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

typedef struct terminal_dimensions {
  int maxx;
  int maxy;
} terminal_dimensions_t;

// Delay timers for the main game loop.
#ifndef DELAY_US
#define DELAY_US 100000
#endif

// Game States
enum {INIT, ADD_PIECE, MOVE_PIECE, ADJUST_WELL, EXIT, GAME_OVER};

void init_game(void);

int game(void);


/* game.hpp ends here */
