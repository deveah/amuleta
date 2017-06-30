
/*
 *  main.c
 *  Part of Amuleta, a traditional roguelike - https://deveah.github.io/amuleta
 *  (c) Vlad Dumitru, <dalv.urtimud@gmail.com>
 *  Licensed under the terms and conditions of the MIT License. Please consult
 *  the LICENSE file included with this project.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termbox.h>
#include "amuleta.h"

/*  whether or not the termbox library has been initialized */
int tb_initialized = 0;

/*  terminal dimensions */
int terminal_width  = 0,
    terminal_height = 0;

/*
 *  map from termbox error code to human-readable string
 *
 *  int tb_error -- termbox error code
 *  char* return -- error string
 */
char *tb_error_to_string(int tb_error) {
  switch (tb_error) {
  case TB_EUNSUPPORTED_TERMINAL:
    return "unsupported terminal";
  case TB_EFAILED_TO_OPEN_TTY:
    return "failed to open TTY";
  case TB_EPIPE_TRAP_ERROR:
    return "pipe trap error";
  default:
    return "unknown error";
  }
}

int main(int argc, char **argv)
{
  /*  initialize termbox */
  int err = tb_init();
  if (err < 0) {
    fprintf(stderr, "Unable to initialize termbox: %s\n", tb_error_to_string(err));
    return -1;
  }

  /*  indicate that termbox is running and save the terminal dimensions */
  tb_initialized  = 1;
  terminal_width  = tb_width();
  terminal_height = tb_height();

  /*  check if the terminal fits the minimum size requirements */
  if ((terminal_width  < MINIMUM_TERMINAL_WIDTH) ||
      (terminal_height < MINIMUM_TERMINAL_HEIGHT)) {
    tb_shutdown();
    fprintf(stderr, "Terminal too small. Minimum terminal size is %ix%i\n",
      MINIMUM_TERMINAL_WIDTH, MINIMUM_TERMINAL_HEIGHT);
    return -1;
  }

  /*  initialize the log file */
  initialize_log();

  struct game *g = NULL;

  /*  if the user has provided a random seed, use that one; if not, use the
   *  current timestamp as a random seed for dungeon generation */
  if (argc < 2) {
    g = initialize_game(time(NULL));
  } else {
    g = initialize_game(atoi(argv[1]));
  }

  /*  run the game */
  run_game(g);

  /*  deallocate the game's resources */
  destroy_game(g);

  /*  terminate termbox, logging, and exit */
  terminate_log();
  tb_shutdown();
  return 0;
}

