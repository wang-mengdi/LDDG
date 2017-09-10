#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <cassert>
#include <gl/glut.h>
#include <vector>
#include <list>
using namespace std;
const int SHOWX = 500, SHOWY = 500;
typedef float Float;
typedef int BYT;
BYT colors[][3] = { { 135,206,235 },{ 255,0,0 },{ 0,255,0 },{ 0,0,255 },{ 255,255,0 },{ 255,0,255 },{ 0,255,255 },{ 128,128,128 },{ 255,255,255 } };
const int SHOW_FPS = 60;
const int WALK_TIME = 10;//how many seconds
const int SAMPLE_FREQ = 2000;
const int SIM_FPS = SAMPLE_FREQ / WALK_TIME;
//SAMPLE_FREQ steps includes SHOW_FPS*WALK_TIME shows
//SAMPLE_FREQ steps shall be done in WALK_TIME seconds
const int SAMPLE_STEP = SAMPLE_FREQ / (WALK_TIME*SHOW_FPS);
const Float step = 1.0 / SAMPLE_FREQ;
bool in_run;//animating a Bezier curve
int current_cursor;//now at where
class Point {
public:
	Float x, y;
	Point(Float _x = 0, Float _y = 0) {
		x = _x;
		y = _y;
	}
	Point turn_clipcoor(void) {//x,y at [-1,1]
		return Point(y * 2 / SHOWY - 1.0, (SHOWX - x) * 2 / SHOWX - 1.0);
	}
	void operator += (const Point &b) { x += b.x, y += b.y; }
	friend Point operator + (Point a, const Point &b) {
		a += b;
		return a;
	}
	friend Point operator - (const Point &a, const Point &b) {
		return Point(a.x - b.x, a.y - b.y);
	}
	Point operator * (Float b) {
		return Point(x*b, y*b);
	}
	friend void print(const Point &p) { printf("(%.2lf %.2lf)", p.x, p.y); }
	friend ostream& operator << (ostream &os, const Point &p) {
		os << "(" << p.x << " " << p.y << ")";
		return os;
	}
};
void glVertexP(const Point &p) {
	glVertex2f(p.x, p.y);
}
vector<Point> trace;
void Plot_Bezier_At(vector<vector<Point> >&P, Float u, bool show=true) {
	assert(P.size() > 0);
	int n = P[0].size();
	assert(P.size() >= n);
	for (int t = 1; t < n; t++) {
		P[t].clear();
		BYT *c = colors[(t - 1) % 8];
		if (in_run&&show) glColor3f(c[0] / 255.0, c[1] / 255.0, c[2] / 255.0);
		for (int i = 0; i + 1 < P[t - 1].size(); i++) {
			P[t].push_back(P[t - 1][i] * (1 - u) + P[t - 1][i + 1] * u);
			if (in_run&&show) {
				glBegin(GL_LINES);
				glVertexP(P[t - 1][i].turn_clipcoor());
				glVertexP(P[t - 1][i + 1].turn_clipcoor());
				glEnd();
			}
		}
	}
	if (n<=1&&show) {
		glPointSize(5);
		glColor3f(1.0, 0, 0);
		glBegin(GL_POINTS);
		for (int i = 0; i < P[0].size(); i++) {
			glVertexP(P[0][i].turn_clipcoor());
		}
		glEnd();
		return;
	}
	if(in_run) trace.push_back(P[n - 1][0]);
	if (show) {
		glPointSize(3);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		if (in_run) {
			for (int i = 0; i < trace.size(); i++) {
				glVertexP(trace[i].turn_clipcoor());
			}
		}
		else {
			glVertexP(P[n - 1][0].turn_clipcoor());
		}
		glEnd();
	}
	if (show) {
		glPointSize(5);
		glColor3f(1.0, 0, 0);
		glBegin(GL_POINTS);
		for (int i = 0; i < P[0].size(); i++) {
			glVertexP(P[0][i].turn_clipcoor());
		}
		glEnd();
	}
}
void Plot_Bezier(vector<vector<Point> > &P, bool show=true) {
	if (in_run) {
		Plot_Bezier_At(P, (Float)current_cursor / SAMPLE_FREQ);
	}
	else {
		for (int i = 0; i <= SAMPLE_FREQ; i++) {
			Plot_Bezier_At(P, (Float)i / SAMPLE_FREQ);
		}
	}
}
vector<vector<Point> > P(1);
void Plot(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	Plot_Bezier(P, true);
	glutSwapBuffers();
}
void Step_Time(int value) {
	if (in_run) {
		current_cursor++;
		if (current_cursor > SAMPLE_FREQ) {
			in_run = false;
			printf("绘图动画完成\n");
			trace.clear();
		}
		if (current_cursor%SAMPLE_STEP == 0) glutPostRedisplay();
		else Plot_Bezier(P, false);
		glutTimerFunc(1000 / SIM_FPS, Step_Time, 1);
	}
	else {
		glutPostRedisplay();
		glutTimerFunc(1000 / SHOW_FPS, Step_Time, 1);
	}
	
}
void Process_Motion(int y, int x) {
	P[0].back() = Point(x, y);
}
void Process_Click(int button, int state, int y, int x) {
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN) {
		P[0].push_back(Point(x, y));
		P.push_back(vector<Point>());
		printf("创建新点\n");
	}
}
void Process_key(unsigned char key, int x, int y) {
	if (key == ' ') {
		if (in_run) {
			in_run = false;
			printf("用户停止绘图动画\n");
			trace.clear();
		}
		else {
			in_run = true;
			current_cursor = 0;
			trace.clear();
			printf("开始生成绘图动画\n");
		}
	}
	else if (key == 'e') {
		P.clear();
		printf("清空当前曲线\n");
		P.push_back(vector<Point>());
		P[0].push_back(Point(0, 0));
	}
}
int main(int argc, char *argv[]) {
	printf("用法:按鼠标左键添加控制点，按空格开始/停止绘制动画，按e清除图像\n");
	P[0].push_back(Point(0, 0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//double buffer
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SHOWX, SHOWY);
	int glut_window = glutCreateWindow("Rasterization");
	glutKeyboardFunc(&Process_key);
	glutMouseFunc(&Process_Click);
	glutPassiveMotionFunc(&Process_Motion);
	glutDisplayFunc(&Plot);
	glutTimerFunc(1000 / SHOW_FPS, Step_Time, 1);
	glutMainLoop();
	return 0;
}