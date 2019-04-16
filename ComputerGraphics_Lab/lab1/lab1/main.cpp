#include <GL/glut.h>

GLint xc=0,yc=0,radius = 400;

class screenPt {
private:
	GLint x, y;
public:
	screenPt() {
		x = y = 0;
	}

	void setCoords(GLint xCoordValue, GLint yCoordValue) {
		x = xCoordValue;
		y = yCoordValue;
	}

	GLint getx() const {
		return x;
	}

	GLint gety() const {
		return y;
	}

	void incrementx() {
		x++;
	}

	void decrementy() {
		y--;
	}
};

void setPixel(GLint xCrood,GLint yCrood) {
	glBegin(GL_POINTS);
	glVertex2i(xCrood, yCrood);
	glEnd();
}

void circleMidpoint(GLint xc, GLint yc, GLint radius) {
	screenPt circPt;
	GLint p = 1 - radius;

	circPt.setCoords(0, radius);

	void circlePlotPoints(GLint, GLint, screenPt);
	circlePlotPoints(xc, yc, circPt);

	while (circPt.getx() < circPt.gety()) {
		circPt.incrementx();
		if (p < 0)
			p += 2 * circPt.getx() + 1;
		else {
			circPt.decrementy();
			p += 2 * (circPt.getx() - circPt.gety()) + 1;
		}
		circlePlotPoints(xc, yc, circPt);
	}
}

void circlePlotPoints(GLint xc, GLint yc, screenPt circPt) {
	setPixel(xc + circPt.getx(), yc + circPt.gety());
	setPixel(xc - circPt.getx(), yc + circPt.gety());
	setPixel(xc + circPt.getx(), yc - circPt.gety());
	setPixel(xc - circPt.getx(), yc - circPt.gety());
	setPixel(xc + circPt.gety(), yc + circPt.getx());
	setPixel(xc - circPt.gety(), yc + circPt.getx());
	setPixel(xc + circPt.gety(), yc - circPt.getx());
	setPixel(xc - circPt.gety(), yc - circPt.getx());
}

void myDisplay()
{
	
	glClear(GL_COLOR_BUFFER_BIT);//GL_COLOR_BUFFER_BIT表示清除颜色
	glMatrixMode(GL_MODELVIEW);
	glColor3f(0.0, 1.0f, 0.0f);	
	circleMidpoint(xc, yc, radius);
	glFlush(); //保证前面的OpenGL命令立即执行（而不是让它们在缓冲区中等待）。
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);//对GLUT进行初始化，这个函数必须在其它的GLUT使用之前调用一次
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); //设置显示方式
	glutInitWindowPosition(100, 100); //设置窗口位置
	glutInitWindowSize(400, 400);//窗口大小
	glutCreateWindow("lab 01"); //根据前面设置的信息创建窗口。参数将被作为窗口的标题。
	glClearColor(1, 1, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-500, 500, -500, 500);
	glutDisplayFunc(&myDisplay); //当需要画图时，请调用myDisplay函数
	glutMainLoop(); //进行一个消息循环
	return 0;
}