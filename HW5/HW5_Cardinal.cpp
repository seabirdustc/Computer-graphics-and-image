#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
using std::vector;
using std::begin;
using std::end;
using std::cin;
using std::cout;

/* -------- 常数定义 --------*/
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int DEFAULT_2D_ZNEAR = -1;
const int DEFAULT_2D_ZFAR = 1;
const int NUM_OF_SEGMENTS = 100; 
const double TENSION = 0.5; 

/* -------- 全局变量 --------*/
vector<vector<GLdouble>> controlVertices;


vector<vector<GLdouble>> interpVerts(vector<GLdouble> &vert0, vector<GLdouble> &vert1, size_t numSegments, vector<GLdouble> &tangs) {
	// 初始化返回值
	vector<vector<GLdouble>> curveVerts;
	curveVerts.reserve(numSegments);

	/* interpolate points on curve path from vert0 to vert1 */
	for (int i = 0; i < numSegments; ++i) {
		double u = (double)i / numSegments;

		// 计算系数
		auto h0 = 2 * pow(u, 3) - 3 * pow(u, 2) + 1;
		auto h1 = -2 * pow(u, 3) + 3 * pow(u, 2);
		auto h2 = pow(u, 3) - 2 * pow(u, 2) + u;
		auto h3 = pow(u, 3) - pow(u, 2);

		// 计算插值点
		auto vx = h0 * vert0[0] + h1 * vert1[0] + h2 * tangs[0] + h3 * tangs[2];
		auto vy = h0 * vert0[1] + h1 * vert1[1] + h2 * tangs[1] + h3 * tangs[3];

		curveVerts.push_back(vector<GLdouble>{vx, vy});
	}

	return curveVerts;
}


vector<GLdouble> calcTangs(vector<GLdouble> &v0, vector<GLdouble> &v1, vector<GLdouble> &v2, vector<GLdouble> &v3, double t) {
	// 计算端点处切向
	vector<GLdouble> tangs{ t * (v2[0] - v0[0]), t * (v2[1] - v0[1]), t * (v3[0] - v1[0]), t * (v3[1] - v1[1]) };
	return tangs;
}


vector<vector<GLdouble>> cardinalSpline(vector<vector<GLdouble>> &ctrlVrts, size_t numSegments, double tension) {
	// 初始化返回值
	vector<vector<GLdouble>> splineVerts;
	splineVerts.reserve(4 * numSegments);

	auto fTangs = calcTangs(ctrlVrts[3], ctrlVrts[0], ctrlVrts[1], ctrlVrts[2], tension);
	auto fInterpVerts = interpVerts(ctrlVrts[0], ctrlVrts[1], numSegments, fTangs);
	splineVerts.insert(end(splineVerts), begin(fInterpVerts), end(fInterpVerts));

	auto tangs = calcTangs(ctrlVrts[0], ctrlVrts[1], ctrlVrts[2], ctrlVrts[3], tension);
	auto interpolatedVerts = interpVerts(ctrlVrts[1], ctrlVrts[2], numSegments, tangs);
	splineVerts.insert(end(splineVerts), begin(interpolatedVerts), end(interpolatedVerts));

	auto lTangs = calcTangs(ctrlVrts[1], ctrlVrts[2], ctrlVrts[3], ctrlVrts[0], tension);
	auto lInterpVerts = interpVerts(ctrlVrts[2], ctrlVrts[3], numSegments, lTangs);
	splineVerts.insert(end(splineVerts), begin(lInterpVerts), end(lInterpVerts));

	return splineVerts;
}


void InitVertexes() {
	cout << "请输入四组节点坐标(x,y) (|x|,|y| <= 400)\n";
	double x, y;
	for (int i = 0; i < 4; i++) {
		controlVertices.emplace_back(vector<GLdouble>(2));
		cin >> x >> y;
		controlVertices[i][0] = x;
		controlVertices[i][1] = y;
	}
}

void handleDisplay() {

	glClear(GL_COLOR_BUFFER_BIT);

	// 画出给定的点
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	for (auto &vert : controlVertices) {
		glVertex2dv(&vert[0]);
	}
	glEnd();

	// 计算样条结果
	auto spline = cardinalSpline(controlVertices, NUM_OF_SEGMENTS, TENSION);

	// 画出结果
	glColor3f(0, 0, 0);
	//glBegin(GL_LINE_STRIP);
	glPointSize(1);
	glBegin(GL_POINTS);
	for (auto &vert : spline) {
		glVertex2d(vert[0], vert[1]);
	}
	glEnd();

	glutSwapBuffers();
}


void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0); 
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-400, 400, -400, 400);
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Cardial Spline Interpolate 图形学作业-pb16060738陈宇轩-4.15");

	InitVertexes();

	glutDisplayFunc(handleDisplay);

	init();

	glutMainLoop();

	return 0;
}