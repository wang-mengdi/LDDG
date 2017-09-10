#pragma once

#include "shared.h"

bool is(Material mask[H][W], int i, int j, Material m);
void Advect(bool board[H][W][4], Material mask[H][W]);
void Collide(bool board[H][W][4], Material mask[H][W]);