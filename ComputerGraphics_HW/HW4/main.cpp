#include <GL/glut.h>
#include <iostream>
#include <vector>
using namespace std;
GLint winWidth = 600, winHeight = 600; // Initial display-window size.
GLfloat x0 = 100.0, y0 = 50.0, z0 = 50.0; // Viewing-coordinate origin.
GLfloat xref = 50.0, yref = 50.0, zref = 0.0; // Look-at point.
GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0; // View-up vector.
/* Set coordinate limits for the clipping window: */
GLfloat xwMin = -40.0, ywMin = -60.0, xwMax = 40.0, ywMax = 60.0;
/* Set positions for near and far clipping planes: */
GLfloat dnear = 25.0, dfar = 125.0;
int front_behind;
double eye_x, eye_y, eye_z, center_x, center_y, center_z, up_x, up_y, up_z;
int num = 3;
vector<Point> vertexes;

class Point
{
public:
	Point(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	double x, y, z;
};

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(x0, y0, z0, xref, yref, zref, Vx, Vy, Vz);
	glMatrixMode(GL_PROJECTION);
	glFrustum(xwMin, xwMax, ywMin, ywMax, dnear, dfar);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	/* Set parameters for a square fill area. */
	glColor3f(0.0, 1.0, 0.0); // Set fill color to green.
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE); // Wire-frame back face.
	glBegin(GL_QUADS);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);
	glVertex3f(100.0, 100.0, 0.0);
	glVertex3f(0.0, 100.0, 0.0);
	glEnd();
	glFlush();
}
void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	winWidth = newWidth;
	winHeight = newHeight;
}


void P1() {
	cout << "输入0视图为正方形前面，输入1视图为正方形后面:";
	cin >> front_behind;
	switch (front_behind)
	{
	case 0:
		z0 = 50;    // 正方形的前面
		break;
	case 1:
		z0 = -50;   // 正方形的后面
		break;
	}
}

void P2() {
	cout << "输入透视观察参数 gluLookAt(double eye_x, double eye_y, double eye_z, double center_x, double center_y, double center_z, "
		<< "double up_x, double up_y, double up_z): ";
	cin >> eye_x >> eye_y >> eye_z >> center_x >> center_y >> center_z >> up_x >> up_y >> up_z;

	x0 = eye_x; y0 = eye_y; z0 = eye_z;
	xref = center_x; yref = center_y; zref = center_z;
	Vx = up_x; Vy = up_y; Vz = up_z;
}

void P3() {
	cout << "输入点的数量 (number >= 3): ";
	cin >> num;
	cout << "输入点的坐标 (x, y, z):" << endl;
	double x, y, z;

	for (int i = 0; i < num; i++)
	{
		cin >> x >> y >> z;
		Point temp(x, y, z);
		vertexes.push_back(temp);
	}

	glColor3d(0, 1, 0);
	glBegin(GL_POLYGON);

	for (auto i : vertexes)
		glVertex3d(i.x, i.y, i.z);

	glEnd();
	
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Perspective View of A Square");
	init();
	int option = 0;
	switch (option) {
	case 1:
		P1();
		break;
	case 2:
		P2();
		break;
	case 3:
		P3();
		break;
	default:
		break;
	}
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(reshapeFcn);
	glutMainLoop();
	return 0;
}
