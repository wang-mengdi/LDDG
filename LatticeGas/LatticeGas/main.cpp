#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <cassert>
#include <gl/glut.h>
#include <vector>
#include <list>
#include "shared.h"
#include "lattice.h"
using namespace std;

class BlastBall {
public:
	int x, y;
	int radius;
	BlastBall(int _x,int _y):x(_x),y(_y),radius(0){}
};

Material mask[H][W];
bool board[H][W][4];
vector<BlastBall> balls;
void setmask(int i, int j, Material m) {
	if (i == 0 || i == H - 1 || j == 0 || j == W - 1) return;
	if (inside(i, j)) {
		mask[i][j] = m;
	}
}
void Step_Blast(void) {
	vector<BlastBall> q;
	for (BlastBall &b : balls) {
		for (int i = 0; i <= b.radius; i++) {
			int d1 = i, d2 = b.radius - i;
			setmask(b.x + d1, b.y + d2, SOLID);
			setmask(b.x + d1, b.y - d2, SOLID);
			setmask(b.x - d1, b.y + d2, SOLID);
			setmask(b.x - d1, b.y - d2, SOLID);
		}
		b.radius++;
		if (b.radius <= BALL_LIM) q.push_back(b);
		else {
			for (int r = 0; r <= b.radius; r++) {
				for (int i = 0; i <= r; i++) {
					int d1 = i, d2 = r - i;
					setmask(b.x + d1, b.y + d2, AIR);
					setmask(b.x + d1, b.y - d2, AIR);
					setmask(b.x - d1, b.y + d2, AIR);
					setmask(b.x - d1, b.y - d2, AIR);
				}
			}
		}
	}
	balls = q;
}

void Process_Click(int button, int state, int y, int x) {
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN){
		printf("add blast\n");
		balls.push_back(BlastBall(x, y));
	}
}

void Plot(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			Float x = i, y = j;
			GridCoor_to_ClipCoor(x, y);
			if (is(mask, i, j, AIR)) {
				int sum = 0;
				for (int d = 0; d < 4; d++) sum += board[i][j][d];
				Float c = sum / 4.0;
				//cout << c << endl;
				glColor3f(c, c, c);
				glVertex2f(x, y);
			}
			else if (i == 0 || i == H - 1 || j == 0 || j == W - 1) {
				glColor3f(1, 0, 0);
				glVertex2f(x, y);
			}
		}
	}
	glEnd();
	glutSwapBuffers();
}

void Init(void) {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (i == 0 || j == 0 || i == H - 1 || j == W - 1) mask[i][j] = SOLID;
			else mask[i][j] = AIR;
			if (mask[i][j] == AIR) {
				for (int d = 0; d < 4; d++) {
					if (random(1, 3) == 1) board[i][j][d] = true;
					else board[i][j][d] = false;
				}
			}
		}
	}
}

void Step_Time(int v) {
	Step_Blast();
	Advect(board, mask);
	Collide(board, mask);
	Plot();
	glutTimerFunc(1000 / SHOW_FPS, Step_Time, 1);
}

int main(int argc, char *argv[]) {
	printf("usage: click to add a blast\n");
	Init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//double buffer
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W, H);
	int glut_window = glutCreateWindow("Rasterization");
	glutMouseFunc(&Process_Click);
	glutDisplayFunc(&Plot);
	glutTimerFunc(1000 / SHOW_FPS, Step_Time, 1);
	glutMainLoop();
	return 0;
}