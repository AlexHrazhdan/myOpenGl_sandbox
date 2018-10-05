#include <GL\glew.h>
#include <GL\glut.h>
#include <iostream>
#include <fstream>
#include <glm.hpp>
#include <QtGui\QMouseEvent>
#include <QtGui\QKeyEvent>
#include <gtc\matrix_transform.hpp>

//#include <gtx\transform.hpp>
#include <meGlWindow.h>
#include <Vertex.h>
#include <ShapeGenerator.h>
#include <Camera.h>
using namespace std;
using glm::vec3;
using glm::mat4;
extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;
GLuint programID;
GLuint numIds;
Camera camera;
GLuint cubeVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;

meGlWindow::meGlWindow()
{

}


bool cheskStatus(GLuint objectID,
				 PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
				 PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
				 GLenum statusType)
{
	GLint compileStatus;
	objectPropertyGetterFunc(objectID, statusType, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return cheskStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return cheskStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void instalShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *adapter[1];
	string tmp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = tmp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	tmp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = tmp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	
	glLinkProgram(programID);
	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glUseProgram(programID);

	
}


void sendDataToOpenGL()
{



	ShapeData shape = ShapeGenerator::makeCube();
	GLuint* vertexBufferId;
	glGenBuffers(2, vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char*)(3 * sizeof(float)));


	GLuint indexBufferId;
	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.idsBufferSize(), shape.indices, GL_STATIC_DRAW);
	numIds = shape.numIndices;
	shape.cleanup();
}

void meGlWindow::setupVertexArrays()
{
	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	glGenVertexArrays(1, &arrowVertexArrayObjectID);

	glBindVertexArray(cubeVertexArrayObjectID);
	glBindVertexArray(arrowVertexArrayObjectID);
}

void meGlWindow::initializeGL()
{
	glewInit();
	setMouseTracking(true);
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	instalShaders();
	setupVertexArrays();
}
void meGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.21f, 0.23f, 1.0f);
	glViewport(0, 0, width(), height());

	mat4 projectionMatrix  = glm::perspective(glm::radians(60.0f), (float)width() / height(), 0.1f, 100.0f);
	mat4 translationMatrix = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -5.0f));
	mat4 rotationMatrix    = glm::rotate(glm::mat4(1), glm::radians(30.0f), vec3(0.0f, 1.0f, 0.0f));
	
	mat4 combinedMat = projectionMatrix*camera.getWorldToViewMatrix()*translationMatrix*rotationMatrix;

	
	GLint transformationMatrixUniformLocatrion =
		glGetUniformLocation(programID, "transformationMatrix");

	glUniformMatrix4fv(transformationMatrixUniformLocatrion, 1, GL_FALSE, &combinedMat[0][0]);

	glDrawElements(GL_TRIANGLES, numIds, GL_UNSIGNED_SHORT, 0);

}

void meGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void meGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		camera.moveForward();
		break;
	case Qt::Key::Key_S:
		camera.moweBackward();
		break;
	case Qt::Key::Key_A:
		camera.moveLeft();
		break;
	case Qt::Key::Key_D:
		camera.moveRight();
		break;
	case Qt::Key::Key_R:
		camera.moveUp();
		break;
	case Qt::Key::Key_F:
		camera.moveDown();
		break;
	}
	repaint();
}

meGlWindow::~meGlWindow()
{
	glUseProgram(0);
	glDeleteProgram(programID);
}
