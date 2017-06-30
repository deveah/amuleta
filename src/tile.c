
/*
 *  tile.c
 *  Part of Amuleta, a traditional roguelike - https://deveah.github.io/amuleta
 *  (c) Vlad Dumitru, <dalv.urtimud@gmail.com>
 *  Licensed under the terms and conditions of the MIT License. Please consult
 *  the LICENSE file included with this project.
 */

#include <termbox.h>
#include "amuleta.h"

struct tb_cell void_tile_cell = {
  .ch = ' ',
  .fg = TB_BLACK,
  .bg = TB_DEFAULT
};

struct tile void_tile = {
  .cell  = &void_tile_cell,
  .flags = TILE_FLAG_SOLID 
};

struct tb_cell floor_tile_cell = {
  .ch = '.',
  .fg = TB_WHITE,
  .bg = TB_DEFAULT
};

struct tile floor_tile = {
  .cell  = &floor_tile_cell,
  .flags = 0
};

struct tb_cell wall_tile_cell = {
  .ch = '#',
  .fg = TB_YELLOW,
  .bg = TB_DEFAULT
};

struct tile wall_tile = {
  .cell  = &wall_tile_cell,
  .flags = TILE_FLAG_SOLID | TILE_FLAG_OPAQUE
};

