
/*
 *  game.c
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
 *  initialize the game, creating the dungeon and the player
 *
 *  unsigned int random_seed -- the random seed used to generate the dungeon
 *  struct game *return      -- the game structure
 */
struct game *initialize_game(unsigned int random_seed)
{
  /*  allocate the game structure */
  struct game *g = (struct game*)malloc(sizeof(struct game));
  assert(g != NULL);

  /*  save the random seed for future reference */
  g->random_seed = random_seed;
  srand(g->random_seed);
  
  /*  generate the dungeon */
  g->dungeon = generate_dungeon();

  /*  generate the player actor entity */
  g->player = create_player();

  /*  the player actor entity is the first one in the `actors' list */
  g->actors = g->player;
  g->player->next = NULL;

  /*  mark the game as not running (yet) */
  g->running = 0;

  return g;
}

/*
 *  destroys a game structure, including all of its allocated data
 *
 *  struct game *g -- the game structure to be deallocated
 */
void destroy_game(struct game *g)
{
  /*  free the dungeon */
  free_dungeon(g->dungeon);

  /*  free all actors */
  struct actor *current = g->actors;
  while (current) {
    struct actor *next = current->next;
    free(current);
    current = next;
  }

  free(g);
}

/*
 *  creates the player actor with default values
 *
 *  struct actor *return -- the actor structure
 */
struct actor *create_player(void)
{
  /*  allocate the actor structure */
  struct actor *a = (struct actor*)malloc(sizeof(struct actor));

  /*  set the name of the player; TODO: ask the user for this */
  a->name = "You";

  /*  set the player's cell appearance */
  struct tb_cell player_cell = {
    .ch = '@',
    .fg = TB_WHITE,
    .bg = TB_DEFAULT
  };

  a->cell = &player_cell;

  /*  hp and max_hp are initialized to 1 as a default, so that the player actor
   *  may be alive */
  a->hp = 1;
  a->max_hp = 1;

  /*  the player's coordinates are by default the center of the topmost level
   */
  a->x = MAP_WIDTH/2;
  a->y = MAP_HEIGHT/2;
  a->z = 0;

  return a;
}

/*
 *  begins playing a game
 *
 *  struct game *g -- the game state structure
 */
void run_game(struct game *g)
{
  struct tb_event ev;

  /*  flag the game as running */
  g->running = 1;

  while (g->running) {
    
    /*  draw the user interface */
    draw_map(g, g->player->z);

    /*  ask for user input */
    tb_poll_event(&ev);

    /*  if the user pressed 'q', exit the game */
    if ((ev.type == TB_EVENT_KEY) && (ev.ch == 'q')) {
      g->running = 0;
    }
  }
}

