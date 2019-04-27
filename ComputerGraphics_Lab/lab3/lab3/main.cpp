#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;


struct Point//顶点结构体
{
	GLfloat xyz[3];
};

struct Material//材质结构体
{
	GLfloat ambient[4];//环境光
	GLfloat diffuse[4];//漫反射
	GLfloat specular[4];//镜面反射
	GLfloat emission[4];//辐射
	GLfloat shiness;//光斑
	int material_index;//纹理文件索引
};

struct Chartlet//贴图结构体
{
	GLfloat xy[2];
};

struct Normal//法线结构体
{
	GLfloat xyz[3];
};

struct Triangle//子模型里面的三角形
{
	GLint index[9];
};

struct subModel//子模型结构体
{
	GLint num;
	int material_index;
	Triangle *TriangleArray;
};

vector<string> textureName;//纹理名的数组
Point *PointArray;
Material *MaterialArray;//给材质数组声明一个指针，后续再为指针分配内存
Chartlet *ChartletArray;
Normal *NormalArray;
subModel *subModelArray;
int length_pointarray;
float scale[3];
int num_submodel;
GLfloat translateX, translateY, translateZ;//平移
GLfloat rotateAngle, rotateX, rotateY, rotateZ;//旋转
GLfloat scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;//缩放
GLfloat fovy = 0, aspect = 0, zFar = 0, zNear = 1.0;//投影
GLfloat eyex = -1.0, eyey = -2.0, eyez = 1.5, centerx = 0.0, centery = 0.0, centerz = 0.0, upx = 0.0, upy = 0.0, upz = 1.0;//三维观察
GLfloat* indexA = NULL, *indexB = NULL, *indexC = NULL, *indexD = NULL;


void funRead()//读取文件总函数
{
	string s;
	stringstream ss;
	int i, j;

	fstream file_name("luweiqi.txt");//打开文件
	if (!file_name) { cout << "Error opening " << endl; exit(-1); }

	//读取纹理文件名
	int numVertex;
	getline(file_name, s);
	ss << s;
	ss >> numVertex;
	cout << "number of vertex is " << numVertex << endl;
	ss.clear();

	for (i = 0; i < numVertex; i++)
	{
		getline(file_name, s);
		textureName.push_back(s);
	}

	//读取材质文件
	int numMaterial;
	getline(file_name, s);
	ss << s;
	ss >> numMaterial;
	cout << "number of materials is " << numMaterial << endl;
	MaterialArray = new Material[numMaterial];//给材质指针分配内存
	ss.clear();

	for (i = 0; i < numMaterial; i++)
	{
		getline(file_name, s);//读取ambient
		ss << s;
		for (j = 0; j < 4; j++)
		{
			ss >> MaterialArray[i].ambient[j];
		}
		ss.clear();

		getline(file_name, s);//读取diffuse
		ss << s;
		for (j = 0; j < 4; j++)
		{
			ss >> MaterialArray[i].diffuse[j];
		}
		ss.clear();

		getline(file_name, s);//读取specular
		ss << s;
		for (j = 0; j < 4; j++)
		{
			ss >> MaterialArray[i].specular[j];
		}
		ss.clear();

		getline(file_name, s);//读取emission
		ss << s;
		for (j = 0; j < 4; j++)
		{
			ss >> MaterialArray[i].emission[j];
		}
		ss.clear();

		getline(file_name, s);//读取shiness
		ss << s;
		ss >> MaterialArray[i].shiness;
		ss.clear();

		getline(file_name, s);//读取index
		ss << s;
		ss >> MaterialArray[i].material_index;
		ss.clear();
	}

	//读取顶点坐标
	getline(file_name, s);
	ss << s;
	int numPoints;
	ss >> numPoints;
	length_pointarray = numPoints;
	cout << "number of points is " << numPoints << endl;
	ss.clear();
	PointArray = new Point[numPoints];

	for (i = 0; i < numPoints; i++)//反复读取num行顶点坐标
	{
		getline(file_name, s);
		ss << s;
		for (j = 0; j < 3; j++)
		{
			ss >> PointArray[i].xyz[j];
		}
		ss.clear();
	}

	//读取贴图文件
	int numChartlet;
	getline(file_name, s);
	ss << s;
	ss >> numChartlet;
	cout << "number of maps is " << numChartlet << endl;
	ss.clear();
	ChartletArray = new Chartlet[numChartlet];

	for (i = 0; i < numChartlet; i++)
	{
		getline(file_name, s);
		ss << s;
		for (j = 0; j < 2; j++)
		{
			ss >> ChartletArray[i].xy[j];
		}
		ss.clear();
	}

	//读取法线
	int numNormal;
	getline(file_name, s);
	ss << s;
	ss >> numNormal;
	cout << "number of Normals is " << numNormal << endl;
	ss.clear();
	NormalArray = new Normal[numNormal];

	for (i = 0; i < numNormal; i++)
	{
		getline(file_name, s);
		ss << s;
		for (j = 0; j < 3; j++)
		{
			ss >> NormalArray[i].xyz[j];
		}
		ss.clear();
	}

	//读取子模型数量
	int numSubModel;
	getline(file_name, s);
	ss << s;
	ss >> numSubModel;
	num_submodel = numSubModel;
	cout << "number of submodels is " << numSubModel << endl;
	ss.clear();
	subModelArray = new subModel[numSubModel];

	//读取缩放系数
	getline(file_name, s);
	ss << s;
	for (i = 0; i < 3; i++)
	{
		ss >> scale[i];
	}
	ss.clear();

	//读取子模型
	for (i = 0; i < numSubModel; i++)
	{
		getline(file_name, s);
		ss << s;
		ss >> subModelArray[i].num;
		ss.clear();

		getline(file_name, s);
		ss << s;
		ss >> subModelArray[i].material_index;
		ss.clear();

		subModelArray[i].TriangleArray = new Triangle[subModelArray[i].num];
		for (j = 0; j < subModelArray[i].num; j++)
		{
			getline(file_name, s);
			ss << s;
			for (int jj = 0; jj < 9; jj++)
			{
				ss >> subModelArray[i].TriangleArray[j].index[jj];
			}
			ss.clear();
		}
	}
}

void display()//显示函数，点模式
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective(fovy * 20, aspect, zFar, zNear);
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);//观察正向
	glTranslatef((GLdouble)translateX, (GLdouble)translateY, (GLdouble)translateZ);
	glRotatef((GLdouble)rotateAngle * 20, (GLdouble)rotateX, (GLdouble)rotateY, (GLdouble)rotateZ);
	glScalef((GLdouble)scaleX, (GLdouble)scaleY, (GLdouble)scaleZ);


	for (int i = 0; i < num_submodel; i++)
	{
		//加载材质文件
		glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialArray[subModelArray[i].material_index - 1].ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialArray[subModelArray[i].material_index - 1].diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, MaterialArray[subModelArray[i].material_index - 1].specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, MaterialArray[subModelArray[i].material_index - 1].emission);
		glMaterialf(GL_FRONT, GL_SHININESS, MaterialArray[subModelArray[i].material_index - 1].shiness);
		int index_tex = MaterialArray[subModelArray[i].material_index - 1].material_index - 1;

		// 加载并生成纹理
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(TRUE);
		unsigned char *data = stbi_load(textureName[index_tex].c_str(), &width, &height, &nrChannels, 0);
		if (!data) cout << "fail to load texture" << endl;
		if (i == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		// 画图
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < subModelArray[i].num; j++)
		{
			glNormal3fv(NormalArray[subModelArray[i].TriangleArray[j].index[2] - 1].xyz);
			glTexCoord2f(ChartletArray[subModelArray[i].TriangleArray[j].index[1] - 1].xy[0], 1 - ChartletArray[subModelArray[i].TriangleArray[j].index[1] - 1].xy[1]);
			glVertex3fv(PointArray[subModelArray[i].TriangleArray[j].index[0] - 1].xyz);

			glNormal3fv(NormalArray[subModelArray[i].TriangleArray[j].index[5] - 1].xyz);
			glTexCoord2f(ChartletArray[subModelArray[i].TriangleArray[j].index[4] - 1].xy[0], 1 - ChartletArray[subModelArray[i].TriangleArray[j].index[4] - 1].xy[1]);
			glVertex3fv(PointArray[subModelArray[i].TriangleArray[j].index[3] - 1].xyz);

			glNormal3fv(NormalArray[subModelArray[i].TriangleArray[j].index[8] - 1].xyz);
			glTexCoord2f(ChartletArray[subModelArray[i].TriangleArray[j].index[7] - 1].xy[0], 1 - ChartletArray[subModelArray[i].TriangleArray[j].index[7] - 1].xy[1]);
			glVertex3fv(PointArray[subModelArray[i].TriangleArray[j].index[6] - 1].xyz);
		}
		glEnd();
	}

	// 画坐标轴
	glBegin(GL_LINES);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -1.0);
	glEnd();

	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, 600, 600);

	glMatrixMode(GL_PROJECTION);
	glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 20.0);

	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);

	float fAmbientColor[] = { 0.2f, 0.0f,0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbientColor);

	float fDiffuseColor[] = { 0.2f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffuseColor);

	float fSpecularColor[] = { 0.2f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, fSpecularColor);

	float fPosition[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, fPosition);

	glEnable(GL_LIGHTING);
}

void keyBoardFunc(unsigned char key, int x, int y)
{
	if (key == 'q')
		if (indexA)*indexA += 0.05;
	if (key == 'a')
		if (indexA)*indexA -= 0.05;
	if (key == 'w')
		if (indexB)*indexB += 0.05;
	if (key == 's')
		if (indexB)*indexB -= 0.05;
	if (key == 'e')
		if (indexC)*indexC += 0.05;
	if (key == 'd')
		if (indexC)*indexC -= 0.05;
	if (key == 'r')
		if (indexD)*indexD += 0.05;
	if (key == 'f')
		if (indexD)*indexD -= 0.0;

	//输出参数方便调整
	cout << "glScale:\t" << scaleX << '\t' << scaleY << '\t' << scaleZ << '\t' << endl;
	cout << "glRotate:\t" << rotateAngle * 20 << '\t' << rotateX << '\t' << rotateY << '\t' << rotateZ << endl;
	cout << "glTranslate:\t" << translateX << '\t' << translateY << '\t' << translateZ << endl;
	cout << "glLookAt eye:\t" << eyex << '\t' << eyey << '\t' << eyez << endl;
	cout << "glLookAtCenter:\t" << centerx << '\t' << centery << '\t' << centerz << endl;
	cout << "glLookAt up:\t" << upx << '\t' << upy << '\t' << upz << endl;
	cout << "glperspective:\t" << fovy * 20 << '\t' << aspect << '\t' << zFar << '\t' << zNear << '\t' << endl << endl;
	display();
}

void chooseMode(GLint menuIteemNum)
{
	switch (menuIteemNum)
	{
	case 0:
		indexA = &translateX; indexB = &translateY; indexC = &translateZ; indexD = NULL; break;
	case 1:
		indexA = &scaleX; indexB = &scaleY; indexC = &scaleZ; indexD = NULL; break;
	case 2:
		indexA = &rotateAngle; indexB = &rotateX; indexC = &rotateY; indexD = &rotateZ; break;
	case 3:
		indexA = &fovy; indexB = &aspect; indexC = &zFar; indexD = &zNear; break;
	case 4:
		indexA = &eyex; indexB = &eyey; indexC = &eyez; indexD = NULL; break;
	case 5:
		indexA = &centerx; indexB = &centery; indexC = &centerz; indexD = NULL; break;
	case 6:
		indexA = &upx; indexB = &upy; indexC = &upz; indexD = NULL; break;
	default:
		indexA = NULL; indexB = NULL; indexC = NULL; indexD = NULL; break;
	}
}

void mouseFunc(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	glutCreateMenu(chooseMode);
	glutAddMenuEntry("translate", 0);
	glutAddMenuEntry("scale", 1);
	glutAddMenuEntry("rotate", 2);
	glutAddMenuEntry("perspective", 3);
	glutAddMenuEntry("glLookAt eye", 4);
	glutAddMenuEntry("glLookAt center", 5);
	glutAddMenuEntry("glLookAt up", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB ); 
	glutInitWindowPosition(100, 100); 
	glutInitWindowSize(600, 600);  
	glutCreateWindow("luweiqi");  
	init(); 

	funRead();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyBoardFunc);
	glutMouseFunc(mouseFunc);

	glutMainLoop();

	return 0;
}