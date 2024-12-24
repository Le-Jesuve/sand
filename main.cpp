#include <cstdlib>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  initscr();
  nodelay(stdscr, true);
  curs_set(0);
  keypad(stdscr, true);
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

  srand(time(NULL));
  int ymax, xmax;
  getmaxyx(stdscr, ymax, xmax);

  MEVENT event;
  bool is_dragging = false;
  // find and move any sand onscreen
  while (true) {
    int r = rand() % 2;
    int c = getch();
    if (c == KEY_MOUSE) {
      if (getmouse(&event) == OK) {
        if (event.bstate & BUTTON1_PRESSED || event.bstate & BUTTON1_CLICKED) {
          is_dragging = true; // Start dragging
        } else if (event.bstate & BUTTON1_RELEASED) {
          is_dragging = false; // Stop dragging
        }
      }
    }
    if (is_dragging == true) {
      //     getmouse(&event);
      mvprintw(event.y, event.x, "...");
      mvprintw(event.y - 1, event.x, "...");
      refresh();
    }

    for (int col = 0; col < ymax - 1; col++) {
      for (int row = 0; row < xmax; row++) {
        char ch = mvinch(col, row) & A_CHARTEXT;
        if (ch == '.' && col != ymax) {
          char chb = mvinch((col + 1), row) & A_CHARTEXT;
          if (chb == ' ') {
            mvprintw(col, row, " ");
            mvprintw((col + 1), row, ".");
            refresh();
          } else if (chb == '.') {
            char chl = mvinch(col, row - 1) & A_CHARTEXT;
            char chr = mvinch(col, row + 1) & A_CHARTEXT;
            char chbl = mvinch(col + 1, row - 1) & A_CHARTEXT;
            char chbr = mvinch(col + 1, row + 1) & A_CHARTEXT;
            if (chl == ' ' && chr == ' ' && chbl == ' ' && chbr == ' ') {
              if (r == 1) {
                mvprintw(col, row, " ");
                mvprintw(col + 1, row - 1, ".");
                refresh();
              } else if (r == 0) {
                mvprintw(col, row, " ");
                mvprintw(col + 1, row - 1, ".");
                refresh();
              }
            } else if (chbl == '.' && chbr == ' ') {
              mvprintw(col, row, " ");
              mvprintw(col + 1, row + 1, ".");
              refresh();
            } else if (chbl == ' ' && chbr == '.') {
              mvprintw(col, row, " ");
              mvprintw(col + 1, row - 1, ".");
              refresh();
            }
          }
        }
      }
    }
    refresh();
  }
  getch();
  endwin();
}
