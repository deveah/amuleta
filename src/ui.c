
/*
 *  ui.c
 *  Part of Amuleta, a traditional roguelike - https://deveah.github.io/amuleta
 *  (c) Vlad Dumitru, <dalv.urtimud@gmail.com>
 *  Licensed under the terms and conditions of the MIT License. Please consult
 *  the LICENSE file included with this project.
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <termbox.h>
#include "amuleta.h"

struct tb_cell
  *default_character_map,
  *highlighted_character_map;

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

  DEBUG("Drawing the screen\n");

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
      DEBUG("Drawing actor @0x%p (%s) at %i, %i\n", current,
        current->name, current->x, current->y);
      tb_put_cell(current->x, current->y, current->cell);
    }
    current = current->next;
  }

  tb_puts(0, MAP_HEIGHT,   highlighted_character_map, "Welcome to Amuleta!");
  tb_puts(0, MAP_HEIGHT+1, default_character_map,     "Please don't die often.");

  /*  present the screen buffer */
  tb_present();

  DEBUG("Finised drawing the screen\n");
}

/*
 *  allocates an array of predefined termbox cells, containing the ASCII
 *  character map in range 0 .. 255, both inclusively
 *
 *  int fg                 -- foreground color
 *  int bg                 -- background color
 *  struct tb_cell *return -- the character map
 */
struct tb_cell *generate_character_map(int fg, int bg)
{
  int i;

  /*  allocate the character map */
  struct tb_cell *charmap = (struct tb_cell*)malloc(sizeof(struct tb_cell) * 256);
  assert(charmap != NULL);
  DEBUG("Allocated character map @0x%p\n", charmap);

  /*  enumerate the characters */
  for (i = 0; i < 256; i++) {
    charmap[i].ch = i;
    charmap[i].fg = fg;
    charmap[i].bg = bg;
  }

  DEBUG("Finished generating character map @0x%p\n", charmap);
  return charmap;
}

/*
 *  deallocates a character map
 *
 *  struct tb_cell *charmap -- the character map
 *  void return
 */
void free_character_map(struct tb_cell *charmap)
{
  free(charmap);
}

/*
 *  draws a string onto the screen using a given character map
 *
 *  int x, y                -- starting coordinates of the string
 *  struct tb_cell *charmap -- the character map
 *  char *s                 -- the string to be drawn
 *  void return
 */
void tb_puts(int x, int y, struct tb_cell *charmap, char *s)
{
  int l = strlen(s);
  int i;

  for (i = 0; i < l; i++) {
    tb_put_cell(x + i, y, &charmap[(int)s[i]]);
  }
}

