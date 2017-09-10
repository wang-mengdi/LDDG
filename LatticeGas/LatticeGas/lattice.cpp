#include "lattice.h"

bool board1[H][W][4];

bool is(Material mask[H][W], int i, int j, Material m) {
	return inside(i, j) && mask[i][j] == m;
}

void Advect(bool board[H][W][4], Material mask[H][W]) {
	memset(board1, 0, sizeof(board1));
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (is(mask, i, j, SOLID)) continue;
			for (int d = 0; d < 4; d++) {
				if (!board[i][j][d]) continue;
				int i1 = i + dx[d], j1 = j + dy[d];
				if (is(mask,i, j, SOLID)) {
					if (is(mask, i1, j1, SOLID)) {
						d = opposite((Direction)d);
						i1 = i + dx[d], j1 = j + dy[d];
					}
				}
				if (is(mask, i1, j1, AIR)) {
					board1[i1][j1][d] = true;
				}
				else if (is(mask, i1, j1, SOLID)) {//hit the wall and go back
					board1[i][j][opposite((Direction)d)] = true;
				}
			}
		}
	}
	memcpy(board, board1, sizeof(board1));
}

void Collide(bool board[H][W][4], Material mask[H][W]) {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (is(mask, i, j, SOLID)) continue;
			if (board[i][j][LEFT] && board[i][j][RIGHT] && !board[i][j][UP] && !board[i][j][DOWN]) {
				board[i][j][UP] = board[i][j][DOWN] = true;
				board[i][j][LEFT] = board[i][j][RIGHT] = false;
			}
			else if (board[i][j][UP] && board[i][j][DOWN] && !board[i][j][LEFT] && !board[i][j][RIGHT]) {
				board[i][j][LEFT] = board[i][j][RIGHT] = true;
				board[i][j][UP] = board[i][j][DOWN] = false;
			}
		}
	}
}