#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

//! ���������� � ����������������� ID
//! ID ��������� ���������
GLuint Program;
//! ID ������� ���������� �����
GLint Unif_color;
float rotate_z = 0.0f;
//! �������� ������ OpenGL, ���� ���� �� ����� � ������� ��� ������
void checkOpenGLerror() {
	GLenum errCode;
	if ((errCode = glGetError()) != GL_NO_ERROR)
		std::cout << "OpenGl error! - " << gluErrorString(errCode) << "\n";
}

//! ������������ ��������
void freeShader()
{
	//! ��������� ����, �� ��������� �������� ���������
	glUseProgram(0);
	//! ������� ��������� ���������
	glDeleteProgram(Program);
}
void resizeWindow(int width, int height)
{
	glViewport(0, 0, width, height);
}

void specialKeys(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP: rotate_z += 5.0f; break;
	case GLUT_KEY_DOWN: rotate_z -= 5.0f; break;
	}
	glutPostRedisplay();
}

//! ���������
void renderTriangle()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	//! �������� ������� � ������
	glRotatef(rotate_z, 0.0, 0.0, 1.0);
	//! ������������� ��������� ��������� �������
	glUseProgram(Program);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(-0.5f, -0.5f);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(0.5f, -0.5f);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(0.0f, 0.5f);
	glEnd();

	glFlush();

	//! ��������� ��������� ���������
	glUseProgram(0);
	checkOpenGLerror();
	glutSwapBuffers();
}

//! ������������� ��������
void initShaderTriangle()
{
	//! �������� ��� ��������
	const char* fsSource =
		"void main() {\n"
		"	gl_FragColor = gl_Color;\n"
		"}\n";
	//! ���������� ��� �������� ��������������� ��������
	GLuint fShader;
	//! ������� ����������� ������
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//! �������� �������� ���
	glShaderSource(fShader, 1, &fsSource, NULL);
	//! ����������� ������
	glCompileShader(fShader);
	int compile_ok;
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok)
	{
		std::cout << "error compile shaders \n";
		int loglen;
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &loglen);
		int realLogLen;
		char* log = new char[loglen];
		glGetShaderInfoLog(fShader, loglen, &realLogLen, log);
		std::cout << log << "\n";
		delete log;
	}
	//! ������� ��������� � ����������� ������� � ���
	Program = glCreateProgram();
	glAttachShader(Program, fShader);
	//! ������� ��������� ���������
	glLinkProgram(Program);
	//! ��������� ������ ������
	int link_ok;
	glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
	}

	checkOpenGLerror();
}

//! ���������
void renderCube()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	//! �������� ������� � ������
	glRotatef(rotate_z, 0.0, 0.0, 1.0);
	//! ������������� ��������� ��������� �������
	glUseProgram(Program);
	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//! �������� ������� � ������
	glUniform4fv(Unif_color, 1, black);
	glBegin(GL_QUADS);	
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(-0.5f, -0.5f);	
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(-0.5f, 0.5f);	
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(0.5f, 0.5f);	
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(0.5f, -0.5f);
	glEnd();

	glFlush();

	//! ��������� ��������� ���������
	glUseProgram(0);
	checkOpenGLerror();
	glutSwapBuffers();
}

//! ������������� ��������
void initShaderTabby()
{
	//! �������� ��� ��������
	const char* fsSource =
		"uniform vec4 color;\n"
		"void main() {\n"
		" if (mod(sqrt(gl_FragCoord.x * gl_FragCoord.x + gl_FragCoord.y * gl_FragCoord.y), 10) < 5.0)\n"
		" gl_FragColor = color;\n"
		" else\n"
		" gl_FragColor = vec4(1.0,0.0,1.0,0.0);\n"
		"}\n";
	//! ���������� ��� �������� ��������������� ��������
	GLuint fShader;
	//! ������� ����������� ������
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//! �������� �������� ���
	glShaderSource(fShader, 1, &fsSource, NULL);
	//! ����������� ������
	glCompileShader(fShader);
	int compile_ok;
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok)
	{
		std::cout << "error compile shaders \n";
		int loglen;
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &loglen);
		int realLogLen;
		char* log = new char[loglen];
		glGetShaderInfoLog(fShader, loglen, &realLogLen, log);
		std::cout << log << "\n";
		delete log;
	}
	//! ������� ��������� � ����������� ������� � ���
	Program = glCreateProgram();
	glAttachShader(Program, fShader);
	//! ������� ��������� ���������
	glLinkProgram(Program);
	//! ��������� ������ ������
	int link_ok;
	glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
	}
	//! ���������� ID �������
	const char* unif_name = "color";
	Unif_color = glGetUniformLocation(Program, unif_name);
	if (Unif_color == -1)
	{
		std::cout << "could not bind uniform " << unif_name << std::endl;
		return;
	}

	checkOpenGLerror();
}

//! ������������� ��������
void initShaderCell()
{
	//! �������� ��� ��������
	const char* fsSource =
		"uniform vec4 color;\n"
		"void main() {\n"
		" if (mod(gl_FragCoord.x * gl_FragCoord.y, 10)<5.0) \n"
		" gl_FragColor = color;\n"
		" else\n"
		" gl_FragColor = vec4(1.0,0.0,1.0,0.0);\n"
		"}\n";
	//! ���������� ��� �������� ��������������� ��������
	GLuint fShader;
	//! ������� ����������� ������
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//! �������� �������� ���
	glShaderSource(fShader, 1, &fsSource, NULL);
	//! ����������� ������
	glCompileShader(fShader);
	int compile_ok;
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok)
	{
		std::cout << "error compile shaders \n";
		int loglen;
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &loglen);
		int realLogLen;
		char* log = new char[loglen];
		glGetShaderInfoLog(fShader, loglen, &realLogLen, log);
		std::cout << log << "\n";
		delete log;
	}
	//! ������� ��������� � ����������� ������� � ���
	Program = glCreateProgram();
	glAttachShader(Program, fShader);
	//! ������� ��������� ���������
	glLinkProgram(Program);
	//! ��������� ������ ������
	int link_ok;
	glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
	}
	//! ���������� ID �������
	const char* unif_name = "color";
	Unif_color = glGetUniformLocation(Program, unif_name);
	if (Unif_color == -1)
	{
		std::cout << "could not bind uniform " << unif_name << std::endl;
		return;
	}

	checkOpenGLerror();
}

//! ������������� ��������
void initShaderRainbowCude()
{
	//! �������� ��� ��������
	const char* fsSource =
		"void main() {\n"
		"	gl_FragColor = gl_Color;\n"
		"}\n";
	//! ���������� ��� �������� ��������������� ��������
	GLuint fShader;
	//! ������� ����������� ������
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//! �������� �������� ���
	glShaderSource(fShader, 1, &fsSource, NULL);
	//! ����������� ������
	glCompileShader(fShader);
	int compile_ok;
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok)
	{
		std::cout << "error compile shaders \n";
		int loglen;
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &loglen);
		int realLogLen;
		char* log = new char[loglen];
		glGetShaderInfoLog(fShader, loglen, &realLogLen, log);
		std::cout << log << "\n";
		delete log;
	}
	//! ������� ��������� � ����������� ������� � ���
	Program = glCreateProgram();
	glAttachShader(Program, fShader);
	//! ������� ��������� ���������
	glLinkProgram(Program);
	//! ��������� ������ ������
	int link_ok;
	glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
	}
	//! ���������� ID �������
	const char* unif_name = "color";
	Unif_color = glGetUniformLocation(Program, unif_name);
	if (Unif_color == -1)
	{
		std::cout << "could not bind uniform " << unif_name << std::endl;
		return;
	}

	checkOpenGLerror();
}

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Simple shaders");
	glClearColor(0, 0, 0, 0);
	//! ����������� ����� �������������� ��������
	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status)
	{
		//! GLEW �� ���������������������
		std::cout << "Error: " << glewGetErrorString(glew_status) << "\n";
		return 1;
	}
	//! ��������� ����������� OpenGL 2.0
	if (!GLEW_VERSION_2_0)
	{
		//! OpenGl 2.0 ��������� �� ��������
		std::cout << "No support for OpenGL 2.0 found\n";
		return 1;
	}
	//! ������������� ��������	
	glutReshapeFunc(resizeWindow);

	//initShaderTriangle();
	//glutDisplayFunc(renderTriangle);

	//initShaderTabby();
	initShaderCell();	
	//initShaderRainbowCude();
	glutDisplayFunc(renderCube);


	glutSpecialFunc(specialKeys);
	glutMainLoop();
	//! ������������ ��������
	freeShader();
}