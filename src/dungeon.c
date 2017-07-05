
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

struct tb_cell rat_cell = {
  .ch = 'r',
  .fg = TB_YELLOW,
  .bg = TB_DEFAULT
};

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
 *  finds a random free tile (ie. a non-solid terrain type) on a given map
 *
 *  struct map *m -- the map structure
 *  int *x        -- pointer to where to store `x'
 *  int *y        -- pointer to where to store `y'
 */
void find_random_free_tile(struct map *m, int *x, int *y)
{
  while (1) {
    *x = rand() % MAP_WIDTH;
    *y = rand() % MAP_HEIGHT;

    if (!(m->tile[*x][*y]->flags & TILE_FLAG_SOLID)) {
      break;
    }
  }
}

/*
 *  populate a map with other entities
 *
 *  struct game *g  -- the game structure to which the actors are attached
 *  int z           -- the level index of the map
 *  void return
 */
void populate_map(struct game *g, int z)
{
  int i;

  struct actor *last_added = g->actors;
  while (last_added->next) {
    last_added = last_added->next;
  }

  for (i = 0; i < 20; i++) {
    int x, y;
    find_random_free_tile(g->dungeon->map[z], &x, &y);

    /*  populate with rats */
    struct actor *rat = malloc(sizeof(struct actor));
    rat->name = "Rat";
    rat->cell = &rat_cell;
    rat->x = x;
    rat->y = y;
    rat->z = z;

    rat->hp = 1;
    rat->max_hp = 1;
    rat->next = NULL;

    last_added->next = rat;
    last_added = last_added->next;
  }
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

