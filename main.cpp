#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

//! Переменные с индентификаторами ID
//! ID шейдерной программы
GLuint Program;
float rotate_z = 0.0f;
//! Проверка ошибок OpenGL, если есть то вывод в консоль тип ошибки
void checkOpenGLerror() {
	GLenum errCode;
	if ((errCode = glGetError()) != GL_NO_ERROR)
		std::cout << "OpenGl error! - " << gluErrorString(errCode) << "\n";
}
//! Инициализация шейдеров
void initShader()
{
	//! Исходный код шейдеров
	const char* fsSource =
		"void main() {\n"
		"	gl_FragColor = gl_Color;\n"
		"}\n";
	//! Переменные для хранения идентификаторов шейдеров
	GLuint fShader;
	//! Создаем фрагментный шейдер
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//! Передаем исходный код
	glShaderSource(fShader, 1, &fsSource, NULL);
	//! Компилируем шейдер
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
	//! Создаем программу и прикрепляем шейдеры к ней
	Program = glCreateProgram();
	glAttachShader(Program, fShader);
	//! Линкуем шейдерную программу
	glLinkProgram(Program);
	//! Проверяем статус сборки
	int link_ok;
	glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
	}

	checkOpenGLerror();
}
//! Освобождение шейдеров
void freeShader()
{
	//! Передавая ноль, мы отключаем шейдрную программу
	glUseProgram(0);
	//! Удаляем шейдерную программу
	glDeleteProgram(Program);
}
void resizeWindow(int width, int height)
{
	glViewport(0, 0, width, height);
}
//! Отрисовка
void render2()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	//! Передаем юниформ в шейдер
	glRotatef(rotate_z, 0.0, 0.0, 1.0);
	//! Устанавливаем шейдерную программу текущей
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

	//! Отключаем шейдерную программу
	glUseProgram(0);
	checkOpenGLerror();
	glutSwapBuffers();
}
void specialKeys(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP: rotate_z += 5.0f; break;
	case GLUT_KEY_DOWN: rotate_z -= 5.0f; break;
	}
	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Simple shaders");
	glClearColor(0, 0, 1, 0);
	//! Обязательно перед инициализацией шейдеров
	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status)
	{
		//! GLEW не проинициализировалась
		std::cout << "Error: " << glewGetErrorString(glew_status) << "\n";
		return 1;
	}
	//! Проверяем доступность OpenGL 2.0
	if (!GLEW_VERSION_2_0)
	{
		//! OpenGl 2.0 оказалась не доступна
		std::cout << "No support for OpenGL 2.0 found\n";
		return 1;
	}
	//! Инициализация шейдеров
	initShader();
	glutReshapeFunc(resizeWindow);
	glutDisplayFunc(render2);
	glutSpecialFunc(specialKeys);
	glutMainLoop();
	//! Освобождение ресурсов
	freeShader();
}