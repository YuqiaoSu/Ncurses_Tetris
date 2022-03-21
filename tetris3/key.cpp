/* key.cpp --- 
 * 
 * Filename: key.cpp
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

/* Code: */
#include <ncurses.h> 
#include "key.hpp"

int read_escape(int *read_char) {
  int c; 
  if ((c = getch()) == ERR) {
    return (NOCHAR);
  }
  else if (c==0x1b) {
    if ((c = getch()) == '[') {
      c=getch();
      switch (c) {
      case 'A':
	return (UP);
	break;
      case 'B':
	return (DOWN);
	break;
      case 'C':
	return (RIGHT);
	break;
      case 'D':
	return (LEFT);
	break;
      default: 
	return (BADESC);
      }
    }
  }
  else {
    *read_char = c;
    return (REGCHAR);
  }
}

/* key.cpp ends here */
