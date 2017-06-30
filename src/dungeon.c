
/*
 *  dungeon.c
 *  Part of Amuleta, a traditional roguelike - https://deveah.github.io/amuleta
 *  (c) Vlad Dumitru, <dalv.urtimud@gmail.com>
 *  Licensed under the terms and conditions of the MIT License. Please consult
 *  the LICENSE file included with this project.
 */

#include <assert.h>
#include <stdlib.h>
#include <termbox.h>
#include "amuleta.h"

/*
 *  generate a dungeon structure, complete with generated maps
 *
 *  struct dungeon *return -- the dungeon structure
 */
struct dungeon *generate_dungeon(void)
{
  /*  allocate dungeon struct */
  struct dungeon *d = (struct dungeon*)malloc(sizeof(struct dungeon));
  assert(d != NULL);
  DEBUG("Allocated dungeon @0x%p\n", d);

  /*  generate maps */
  int i;
  for (i = 0; i < DUNGEON_DEPTH; i++) {
    d->map[i] = generate_map();
  }

  DEBUG("Finished creating the dungeon\n");
  return d;
}

/*
 *  generate a map structure, complete with a level layout
 *
 *  struct map *return -- the map structure
 */
struct map *generate_map(void)
{
  int i, j;

  /*  allocate map struct */
  struct map *m = (struct map*)malloc(sizeof(struct map));
  assert(m != NULL);
  DEBUG("Allocated map @0x%p\n", m);

  /*  basic map generation -- fill the map with floor tiles, and border the
   *  level with wall tiles */
  for (i = 0; i < MAP_WIDTH; i++) {
    for (j = 0; j < MAP_HEIGHT; j++) {
      if ((i == 0) || (i == MAP_WIDTH-1) ||
          (j == 0) || (j == MAP_HEIGHT-1)) {
        m->tile[i][j] = &wall_tile;
      } else {
        m->tile[i][j] = &floor_tile;
      }
    }
  }

  DEBUG("Finished generating the map\n");
  return m;
}

/*
 *  free a dungeon structure, together with its attached maps
 *
 *  struct dungeon *d -- the dungeon structure
 *  void return
 */
void free_dungeon(struct dungeon *d)
{
  /*  free the attached maps */
  int i;
  for (i = 0; i < DUNGEON_DEPTH; i++) {
    free(d->map[i]);
    DEBUG("Deallocated map @0x%p (%i)\n", d->map[i], i);
  }

  /*  free the dungeon structure */
  free(d);
  DEBUG("Deallocated dungeon @0x%p\n", d);
}

