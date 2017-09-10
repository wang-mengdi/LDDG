#include "shared.h"

Direction opposite(Direction d) {
	if (d == LEFT) return RIGHT;
	else if (d == RIGHT) return LEFT;
	else if (d == UP) return DOWN;
	else if (d == DOWN) return UP;
	else throw d;
}

bool inside(int i, int j) {
	return 0 <= i&&i < H && 0 <= j&&j < W;
}

int random(int x, int y) {
	return rand() % (y - x + 1) + x;
}

void GridCoor_to_ClipCoor(Float &i, Float &j) {
	Float y = (H - 1 - i)*2.0 / H - 1.0f;
	Float x = j*2.0 / H - 1.0f;
	i = x;
	j = y;
}