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
	
	glClear(GL_COLOR_BUFFER_BIT);//GL_COLOR_BUFFER_BIT��ʾ�����ɫ
	glMatrixMode(GL_MODELVIEW);
	glColor3f(0.0, 1.0f, 0.0f);	
	circleMidpoint(xc, yc, radius);
	glFlush(); //��֤ǰ���OpenGL��������ִ�У��������������ڻ������еȴ�����
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);//��GLUT���г�ʼ�����������������������GLUTʹ��֮ǰ����һ��
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); //������ʾ��ʽ
	glutInitWindowPosition(100, 100); //���ô���λ��
	glutInitWindowSize(400, 400);//���ڴ�С
	glutCreateWindow("lab 01"); //����ǰ�����õ���Ϣ�������ڡ�����������Ϊ���ڵı��⡣
	glClearColor(1, 1, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-500, 500, -500, 500);
	glutDisplayFunc(&myDisplay); //����Ҫ��ͼʱ�������myDisplay����
	glutMainLoop(); //����һ����Ϣѭ��
	return 0;
}