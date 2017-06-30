
/*
 *  ui.c
 *  Part of Amuleta, a traditional roguelike - https://deveah.github.io/amuleta
 *  (c) Vlad Dumitru, <dalv.urtimud@gmail.com>
 *  Licensed under the terms and conditions of the MIT License. Please consult
 *  the LICENSE file included with this project.
 */

#include <termbox.h>
#include "amuleta.h"

/*
 *  draw a map on the screen
 *
 *  struct game *g -- the game structure which contains the map
 *  int z          -- the depth of the map to draw
 *  void return
 */
void draw_map(struct game *g, int z)
{
  struct map *m = g->dungeon->map[z];
  int i, j;

  /*  clear the screen before drawing */
  tb_clear();

  /*  draw the tiles contained by the map */
  for (i = 0; i < MAP_WIDTH; i++) {
    for (j = 0; j < MAP_HEIGHT; j++) {
      tb_put_cell(i, j, m->tile[i][j]->cell);
    }
  }

  /*  draw the actors on this map */
  struct actor *current = g->actors;
  while (current) {
    if (current->z == z) {
      tb_put_cell(current->x, current->y, current->cell);
    }
    current = current->next;
  }

  /*  present the screen buffer */
  tb_present();
}

