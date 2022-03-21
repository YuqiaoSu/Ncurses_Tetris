/* game.cpp ---
 *
 * Filename: game.cpp
 * Description:
 * Author: Bryce
 * Maintainer: Adeel Bhutta
 * Created: Tue Sep  6 11:08:59 2016
 * Last-Updated: 01-10-2021
 *           By: Adeel Bhutta
 *     Update #: 20
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
#include <unistd.h>
#include <ncurses.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>

#include "game.hpp"
#include "well.hpp"
#include "tetris.hpp"
#include "tetromino.hpp"
#include "key.hpp"
#include "score.hpp"

using namespace std;

#define RED 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4

void init_game(void)
{
  int x, y;
}

int game(void)
{
  static int state = INIT;
  tetromino_t *next = NULL;
  tetromino_t *current = NULL;
  well_t *w;

  int x, y;
  int c;
  int arrow;
  struct timespec tim = {0, 1000000}; // Each execution of while(1) is approximately 1mS
  struct timespec tim_ret;
  int move_counter = 0;
  int move_timeout = BASE_FALL_RATE;

  int colorInfo[WELL_WIDTH][WELL_HEIGHT] = {0};
  int baseInfo[WELL_WIDTH][WELL_HEIGHT] = {3};

  // if life become 0, end game.
  int life = 3;

  // previous scores;
  vector<record> records;

  int result;
  bool pause = false;
  int colors = 1;

  int score = 0;
  string name = "";

  int cleared = 0;

  int timer = 1;
  int time_counter = 0;
  mvprintw(0, 0, "time: %d :  %d", timer / 60, timer % 60);

  // load previous records, must before while loop
  loadScore(records);

  int xdirection = 1;
  bool move_flag = false;

  cout << "please enter your name: " << endl;
  cin >> name;

  while (1)
  {

    if (time_counter++ >= 1000)
    {
      // give the obstruction a random moving direction
      if(rand() % 9 > 5){
        xdirection = -1;
      }
      else{
        xdirection = 1;
      }
      mvprintw(0, 0, " ");
      timer++;
      mvprintw(0, 0, "time: %d :    ", timer / 60, timer % 60);
      mvprintw(0, 0, "time: %d :  %d", timer / 60, timer % 60);
      time_counter = 0;
    }
    mvprintw(1, 0, "Score:    ", score);
    mvprintw(1, 0, "Score:  %d", score);

    switch (state)
    {
    case INIT: // Initialize the game, only run one time
      for (int i = 0; i < WELL_WIDTH; i++)
      {
        for (int j = 0; j < WELL_HEIGHT; j++)
        {
          colorInfo[i][j] = 0;
        }
      }
      initscr();
      start_color();
      init_pair(RED, COLOR_RED, COLOR_RED);
      init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);
      init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);
      init_pair(YELLOW, COLOR_YELLOW, COLOR_YELLOW);
      nodelay(stdscr, TRUE);  // Do not wait for characters using getch.
      noecho();               // Do not echo input characters
      getmaxyx(stdscr, y, x); // Get the screen dimensions
      initialize_well(colorInfo);
      initialize_base(baseInfo);
      w = init_well(((x / 2) - (WELL_WIDTH / 2)), 1, WELL_WIDTH, WELL_HEIGHT);
      draw_well(w);

      // print life remaining
      mvprintw(2, 0, "Life Remain:  %d", life);

      // print the score
      printScore(records, y / 5 + 3, x - x / 4);

      srand(time(NULL)); // Seed the random number generator with the time. Used in create tet.
      state = ADD_PIECE;
      break;
    case ADD_PIECE: // Add a new piece to the game

      // add score after pruning the well
      cleared = prune_well(w, colorInfo, baseInfo);
      clean_well(w);
      redraw_well(w, colorInfo);
      if (cleared != 0)
      {
        score = compute_score(score, cleared);
        cleared = 0;
      }

      // change colors
      colors++;
      if (colors == 5)
      {
        colors = 1;
      }

      if (next)
      {
        unprint_tetromino(next, y / 5, x - x / 4);
        current = next;
        next = create_tetromino((w->upper_left_x + (w->width / 2)), w->upper_left_y);
      }
      else
      {
        current = create_tetromino((w->upper_left_x + (w->width / 2)), w->upper_left_y);
        next = create_tetromino((w->upper_left_x + (w->width / 2)), w->upper_left_y);
      }

      // if the piece cannot move when it is generated, the game is over
      if (move_tet(current, current->upper_left_x, current->upper_left_y) == MOVE_FAILED)
      {
        life--;
        if (life == 0)
        {
          state = GAME_OVER;
          break;
        }
        else
        {
          // preserve time and score
          clean_well(w);
          state = INIT;
          break;
        }
      }

      // add colors to the shape, print the current
      attron(COLOR_PAIR(colors));
      display_tetromino(current);
      attroff(COLOR_PAIR(colors));

      // add the colors to the next shape and print the next shape in the side of the window

      print_tetromino(next, y / 5, x - x / 4, colors);

      state = MOVE_PIECE;
      break;
    case MOVE_PIECE: // Move the current piece
      if ((arrow = read_escape(&c)) != NOCHAR)
      {
        undisplay_tetromino(current); // undispay the tetromino
        switch (arrow)
        {
        case UP:
          mvprintw(10, 10, "UP            ");
          rotate_cw(current); // rotate the current tetromino clockwise
          break;
        case DOWN:
          mvprintw(10, 10, "DOWN          ");
          rotate_ccw(current); // rotate the current tetromino counter-clockwise
          break;
        case LEFT:
          mvprintw(10, 10, "LEFT          ");
          result = move_tet(current, current->upper_left_x - 1, current->upper_left_y); // move the tetromino left by 1
          break;
        case RIGHT:
          mvprintw(10, 10, "RIGHT         ");
          result = move_tet(current, current->upper_left_x + 1, current->upper_left_y); // move the tetromino right by 1
          break;
        case REGCHAR:
          mvprintw(10, 10, "REGCHAR 0x%02x", c);
          if (c == 'q')
          {
            state = EXIT;
          }

          if (c == ' ')
          { // if the input command is spacebar
            undisplay_tetromino(current);
            result = MOVE_OK;
            while (result == MOVE_OK)
            {
              result = move_tet(current, current->upper_left_x, current->upper_left_y + 1); // drop the tetromino
            }
            // store_color(current,colorPos, colors);
            attron(COLOR_PAIR(colors));
            display_tetromino(current);
            attroff(COLOR_PAIR(colors));
          }
          if (c == 'p')
          {                 // if the p is pressed here
            pause = !pause; // change the state of the pause
            c = 0;
            do
            {
              read_escape(&c);
              attron(COLOR_PAIR(colors));
              display_tetromino(current);
              attroff(COLOR_PAIR(colors));
            } while (c != 'p');
            pause = !pause;
          }
        }
        attron(COLOR_PAIR(colors));
        display_tetromino(current);
        attroff(COLOR_PAIR(colors));
      }
      if (!pause)
      {
        if (move_counter++ >= move_timeout)
        {
          undisplay_tetromino(current);

          redraw_well(w, colorInfo);

          if (move_tet(current, current->upper_left_x, current->upper_left_y + 1) != MOVE_OK)
          {
            move_timeout = BASE_FALL_RATE - rand() % 400; // random the fall rate after one is droped to the bottom
            state = ADD_PIECE;

            //store the color and base
            store_color(current, colorInfo, colors, w->upper_left_x, w->upper_left_y);
            store_base(current, baseInfo, w->upper_left_x, w->upper_left_y);

            // random between 0,1,2
            move_flag = move_obstruction(colorInfo, baseInfo, xdirection);

            saveScore(records);
          }
          attron(COLOR_PAIR(colors));
          display_tetromino(current);
          attroff(COLOR_PAIR(colors));
          move_counter = 0;
        }
      }
      break;
    case GAME_OVER:  // when the game is over
      clean_well(w); // clean the well
      // Life Remain is 0
      mvprintw(2, 0, "No Life Remains!", life);
      // print gameover inside the well
      mvprintw(WELL_HEIGHT / 2, ((x / 2) - (WELL_WIDTH / 2)) + WELL_WIDTH / 2, "Game Over");
      // save this score
      save_record(name, score, records);
      printScore(records, y / 5 + 3, x - x / 4);
      c = ' ';

      while (1)
      { // let the game keep showing the game over until q(quit) or r(replay) is pressed
        read_escape(&c);
        if (c == 'q')
        {
          state = EXIT;
          break;
        }
        if (c == 'r')
        {
          state = INIT;
          clean_well(w);
          timer = 1;
          life = 3;
          score = 0;
          break;
        }
        if (c == 't')
        {
          //this is the testing button
          clean_well(w);
          redraw_well2(w, baseInfo);
          // break;
        }
      }
      break;
    case EXIT:
      endwin();
      return (0);
      break;
    }
    refresh();
    nanosleep(&tim, &tim_ret);
  }
}

/* game.cpp ends here */
