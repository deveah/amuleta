
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

/*  the player actor's appearance */
struct tb_cell player_cell = {
  .ch = '@',
  .fg = TB_WHITE,
  .bg = TB_DEFAULT
};

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
  DEBUG("Allocated game structure @0x%p\n", g);

  /*  save the random seed for future reference */
  g->random_seed = random_seed;
  srand(g->random_seed);
  INFO("Random seed is %i\n", g->random_seed);
  
  /*  generate the dungeon */
  g->dungeon = generate_dungeon();

  /*  generate the player actor entity */
  g->player = create_player();

  /*  the player actor entity is the first one in the `actors' list */
  g->actors = g->player;
  g->player->next = NULL;

  /*  mark the game as not running (yet) */
  g->running = 0;

  DEBUG("Finished initializing game structure\n");
  return g;
}

/*
 *  destroys a game structure, including all of its allocated data
 *
 *  struct game *g -- the game structure to be deallocated
 *  void return
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
  DEBUG("Deallocated game structure @0x%p\n", g);
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
  assert (a != NULL);
  DEBUG("Allocated player structure @0x%p\n", a);

  /*  set the name of the player; TODO: ask the user for this */
  a->name = "You";

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
 *  void return
 */
void run_game(struct game *g)
{
  /*  flag the game as running */
  g->running = 1;
  INFO("Started game session\n");

  while (g->running) {
    
    /*  loop through all the actors in the game, and make them act */
    struct actor *current = g->actors;

    while (current) {
      DEBUG("Current turn: actor @0x%p (%s)\n", current, current->name);
      do_act(g, current);

      /*  check for an early game exit request */
      if (!g->running) {
        break;
      }

      current = current->next;
    }
  }

  INFO("Ended game session\n");
}

/*
 *  handles a key press event
 *
 *  struct tb_event *ev -- the termbox event structure
 *  void return
 */
void handle_key(struct game *g, struct tb_event *ev)
{
  /*  if the event is not a key press event, do nothing */
  if (ev->type != TB_EVENT_KEY) {
    return;
  }

  DEBUG("Handling key '%c' (code %i)\n", ev->ch, ev->ch);

  /*  handle an exit request */
  if (ev->ch == 'Q') {
    DEBUG("User requested exit\n");
    g->running = 0;
  }

  /*  handle movement */
  if ((ev->key == TB_KEY_ARROW_UP) || (ev->ch == 'k')) {
    move_actor(g, g->player,  0, -1);
  } else if ((ev->key == TB_KEY_ARROW_DOWN) || (ev->ch == 'j')) {
    move_actor(g, g->player,  0,  1);
  } else if ((ev->key == TB_KEY_ARROW_LEFT) || (ev->ch == 'h')) {
    move_actor(g, g->player, -1,  0);
  } else if ((ev->key == TB_KEY_ARROW_RIGHT) || (ev->ch == 'l')) {
    move_actor(g, g->player,  1,  0);
  }
}

/*
 *  makes an actor act; if the actor is player-controlled, act depending on
 *  the user's input; if not, let the computer make the actor complete its turn
 *
 *  struct game *g  -- the game structure
 *  struct actor *a -- the actor in question
 *  void return
 */
void do_act(struct game *g, struct actor *a)
{
  if (a == g->player) {
    /*  if the actor in question is the player, draw the interface, ask for
     *  input, and then act accordingly */
    struct tb_event ev;

    draw_map(g, g->player->z);
    tb_poll_event(&ev);
    handle_key(g, &ev);
  } else {
    /*  TODO */
  }
}

/*
 *  attempts to move the given actor relx units horizontally and rely units
 *  vertically; if the destination does not support an actor, do nothing
 *
 *  struct game *g  -- the game state
 *  struct actor *a -- the actor in question
 *  int relx, rely  -- relative (x, y) coordinates
 *  void return
 */
void move_actor(struct game *g, struct actor *a, int relx, int rely)
{
  /*  an actor cannot move on a solid tile */
  if (g->dungeon->map[a->z]->tile[a->x + relx][a->y + rely]->flags & TILE_FLAG_SOLID) {
    DEBUG("Actor @0x%p (%s) tried to move onto a solid tile: (%i, %i)\n", a,
      a->name, a->x + relx, a->y + rely);
    return;
  }

  /*  update the coordinates */
  a->x += relx;
  a->y += rely;
}

