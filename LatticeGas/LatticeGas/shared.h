#pragma once
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;



typedef float Float;

const int SHOW_FPS = 50;

enum Material { AIR, SOLID };
enum Direction { LEFT, RIGHT, UP, DOWN };
const int W = 500, H = 500;

const int dx[4] = { -1,1,0,0 }, dy[4] = { 0,0,-1,1 };

const int BALL_LIM = 80;

Direction opposite(Direction d);
bool inside(int i, int j);
int random(int x, int y);
void GridCoor_to_ClipCoor(Float &x, Float &y);