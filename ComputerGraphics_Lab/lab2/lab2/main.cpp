#include <iostream>
#include <GL/glut.h>//freeGLUT窗口库


GLfloat v[4][3] = { {0.0,       0.0,       1.0},
				   {0.0,       0.942809,  -0.33333},
				   {-0.816497, -0.471405, -0.333333},
				   {0.816497,  -0.471405, -0.333333} };
GLfloat colors[4][3] = { 0.9647, 0.5764, 0.0784,       // F69314
						0.7686, 0.0431, 0.0745,       // C40B13
						0.3843, 0.0705, 0.5843,       // 621295
						0.0000, 0.4588, 0.9647 };      // 0075F6
int n = 3;

void Trans() {
	glLoadIdentity();
	//    glTranslatef(-0.1,-0.5,0.25);
	//    glRotatef(140,0.6,0.3,-0.5);
	//    glRotatef(50,1,-1,1);
	//    glScalef(1.2,0.3,0.5);

}

void init() {
	glClearColor(0.9529, 0.8274, 0.6784, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	Trans();
	glOrtho(-1, 1, -1, 1, -1, 1);
}

void triangle(GLfloat *va, GLfloat *vb, GLfloat *vc) {
	//画三角形
	glVertex3fv(va);
	glVertex3fv(vb);
	glVertex3fv(vc);
}


void tetra(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d) { //画四面体
	glColor3fv(colors[0]);
	triangle(a, b, c);
	glColor3fv(colors[1]);
	triangle(a, c, d);
	glColor3fv(colors[2]);
	triangle(a, d, b);
	glColor3fv(colors[3]);
	triangle(b, d, c);
}


void divide_tetra(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d, int m) {
	GLfloat mid[6][3];
	int j;
	if (m > 0) {
		/* compute six midpoints */
		for (j = 0; j < 3; j++) mid[0][j] = (a[j] + b[j]) / 2;
		for (j = 0; j < 3; j++) mid[1][j] = (a[j] + c[j]) / 2;
		for (j = 0; j < 3; j++) mid[2][j] = (a[j] + d[j]) / 2;
		for (j = 0; j < 3; j++) mid[3][j] = (b[j] + c[j]) / 2;
		for (j = 0; j < 3; j++) mid[4][j] = (c[j] + d[j]) / 2;
		for (j = 0; j < 3; j++) mid[5][j] = (b[j] + d[j]) / 2;
		/* create 4 tetrahedrons by subdivision */
		divide_tetra(a, mid[0], mid[1], mid[2], m - 1);
		divide_tetra(mid[0], b, mid[3], mid[5], m - 1);
		divide_tetra(mid[1], mid[3], c, mid[4], m - 1);
		divide_tetra(mid[2], mid[4], d, mid[5], m - 1);
	}
	else {
		tetra(a, b, c, d); /* draw tetrahedron at end of recursion */
	}
}

void display() {
	// 清空颜色和深度缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	divide_tetra(v[0], v[1], v[2], v[3], n);
	glEnd();
	glFlush();
}


int main(int argc, char **argv) {

	// 初始化GLUT
	glutInit(&argc, argv);

	// 显示模式
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	// 窗口设置
	glutInitWindowSize(640, 640);      // 窗口尺寸
	glutInitWindowPosition(100, 100);  // 窗口位置
	glutCreateWindow("3D Triangle");   // 窗口标题

	// 开始渲染
	glutDisplayFunc(display);

	init();

	// 激活z-buffer隐藏面消除算法
	glEnable(GL_DEPTH_TEST);

	// 通知开始GLUT的内部循环
	glutMainLoop();

	return 0;
}