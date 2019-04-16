#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
using std::vector;
using std::begin;
using std::end;
using std::cin;
using std::cout;

const GLint winWidth = 600, winHeight = 600; // Initial display-window size.
const int NUM_OF_SEGMENTS = 100;

vector<vector<GLdouble>> controlVertices;


vector<vector<GLdouble>> BezierSpline(vector<vector<GLdouble>> &ctrlVrts, size_t numSegments) {
	// 初始化返回值
	vector<vector<GLdouble>> splineVerts;
	splineVerts.reserve(numSegments);

	for (int i = 0; i < numSegments; ++i) {
		double u = (double)i / numSegments;

		// 计算系数
		auto BEZ0 = pow(1 - u, 3);
		auto BEZ1 = 3 * u*pow(1 - u, 2);
		auto BEZ2 = 3 * u*u*(1 - u);
		auto BEZ3 = pow(u, 3);

		// 计算插值点
		auto vx = BEZ0 * ctrlVrts[0][0] + BEZ1 * ctrlVrts[1][0] + BEZ2 * ctrlVrts[2][0] + BEZ3 * ctrlVrts[3][0];
		auto vy = BEZ0 * ctrlVrts[0][1] + BEZ1 * ctrlVrts[1][1] + BEZ2 * ctrlVrts[2][1] + BEZ3 * ctrlVrts[3][1];
		auto vz = BEZ0 * ctrlVrts[0][2] + BEZ1 * ctrlVrts[1][2] + BEZ2 * ctrlVrts[2][2] + BEZ3 * ctrlVrts[3][2];

		splineVerts.push_back(vector<GLdouble>{vx, vy, vz});
	}
	return splineVerts;
}


void InitVertexes() {
	cout << "请输入四个控制点坐标(x,y,z) \n";
	double x, y, z;
	for (int i = 0; i < 4; i++) {
		controlVertices.emplace_back(vector<GLdouble>(3));
		cin >> x >> y >> z;
		controlVertices[i][0] = x;
		controlVertices[i][1] = y;
		controlVertices[i][2] = z;

	}
}

void handleDisplay() {

	glClear(GL_COLOR_BUFFER_BIT);

	// 画出给定的点
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	for (auto &vert : controlVertices) {
		glVertex3dv(&vert[0]);
	}
	glEnd();

	// 计算Bezier 样条曲线
	auto Bezier = BezierSpline(controlVertices, NUM_OF_SEGMENTS);

	// 画出结果
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glPointSize(1);
	//glBegin(GL_POINTS);
	for (auto &vert : Bezier) {
		glVertex3f(vert[0], vert[1], vert[2]);
	}
	glEnd();

	glFlush();
}


void init() {
	GLfloat x0 = 100.0, y0 = 50.0, z0 = 50.0; // Viewing-coordinate origin.
	GLfloat xref = 50.0, yref = 50.0, zref = 0.0; // Look-at point.
	GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0; // View-up vector.
	/* Set coordinate limits for the clipping window: */
	GLfloat xwMin = -40.0, ywMin = -60.0, xwMax = 40.0, ywMax = 60.0;
	/* Set positions for near and far clipping planes: */
	GLfloat dnear = 25.0, dfar = 125.0;

	int op;

	// 用户自定义输入控制点
	InitVertexes();

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	cout << "透视投影输入1 正交投影输入2:\n";
	cin >> op;
	switch (op)
	{
	case 1: {
		//用户输入透视观察参数
		double eye_x, eye_y, eye_z, center_x, center_y, center_z, up_x, up_y, up_z;
		cout << "输入透视观察参数 gluLookAt(double eye_x, double eye_y, double eye_z, double center_x, double center_y, double center_z, "
			<< "double up_x, double up_y, double up_z): \n";
		cin >> eye_x >> eye_y >> eye_z >> center_x >> center_y >> center_z >> up_x >> up_y >> up_z;
		x0 = eye_x; y0 = eye_y; z0 = eye_z;
		xref = center_x; yref = center_y; zref = center_z;
		Vx = up_x; Vy = up_y; Vz = up_z;
		gluLookAt(x0, y0, z0, xref, yref, zref, Vx, Vy, Vz);
		glMatrixMode(GL_PROJECTION);
		glFrustum(xwMin, xwMax, ywMin, ywMax, dnear, dfar);
		break;
	}
	case 2: {
		// 正交投影
		GLdouble left, right, bottom, top, zNear, zFar;
		cout << "输入正交投影参数 glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar): \n";
		cin >> left >> right >> bottom >> top >> zNear >> zFar;
		glOrtho(left, right, bottom, top, zNear, zFar);
		break;
	}
	default:
		glOrtho(-200,200,-200,200,-200,200);
		break;

	}


}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Bezier Curve 图形学作业-pb16060738陈宇轩-4.15");
	init();
	glutDisplayFunc(handleDisplay);
	glutMainLoop();

	return 0;
}