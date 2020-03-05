#define GLEW_STATIC

#include <GL/glew.h>
#include "MyGlWindow.h"

#include <cassert>
#include <QtCore/qdebug>
#include <QTGui/QMouseEvent>
#include <QTGui/QKeyEvent>
#include <fstream>
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <Primitives/vertex.h>
#include <Primitives/ShapeData.h>
#include <Primitives/ShapeGenerator.h>

GLuint programID;
using glm::vec3;
using glm::mat4;

void MyGlWindow::sendDataToOpenGL() {
	ShapeData Shape = ShapeGenerator::makeCube();

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, Shape.vertexBufferSize(), Shape.Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	GLuint offsetMatrixBufferID;
	glGenBuffers(1, &offsetMatrixBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, offsetMatrixBufferID);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(mat4), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float)*0));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float)*4));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float)*8));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float)*12));
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	/*GLfloat offsets[] = { 0.0f,5.0f };
	GLuint offsetBufferID;
	glGenBuffers(1, &offsetBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, offsetBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(offsets), offsets, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(2, 1);*/

	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Shape.indexBufferSize(), Shape.Indices , GL_STATIC_DRAW);
	
	Shape.cleanup();
}

bool MyGlWindow::checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType
) {
	GLint compileStatus;
	objectPropertyGetterFunc(objectID, statusType, &compileStatus);
	if (compileStatus != GL_TRUE) {
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];
		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		qDebug() << buffer;
		delete[] buffer;
		return false;
	}
	return true;
}

bool MyGlWindow::checkShaderStatus(GLuint shaderID) {
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool MyGlWindow::checkProgramStatus(GLuint programID) {
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string MyGlWindow::readShaderCode(const char* fileName) {
	std::ifstream meInput(fileName);
	if (!meInput.good()) {
		qDebug() << "File Failed to load ..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void MyGlWindow::installShaders() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	std::string temp = readShaderCode(".\\Shaders\\VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
				 temp = readShaderCode(".\\Shaders\\FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);
	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID)) {
		return;
	}
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	if (!checkProgramStatus(programID)) {
		return;
	}
	glUseProgram(programID);
}

void MyGlWindow::initializeGL() {
	setCursor(Qt::BlankCursor);
	setMouseTracking(true);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
	connect(&myTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
	myTimer.start(17);  //60fps
}
void MyGlWindow::myUpdate() {
	myClock.clockGetNewFrame();
	float frameTime = myClock.clockTimeLastFrame();
	keyPressEvent();
	repaint();
}

void MyGlWindow::paintGL() {
	glClear(GL_DEPTH_BUFFER_BIT| GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	mat4 projectionMatrix = glm::perspective(glm::radians(60.0f),
		static_cast<float>(width()) / height(), 0.1f, 50.0f);

	mat4 TransformMatrix[2] = {
		projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(vec3(-5.0f, 0.0f, -10.0f)),
		projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(vec3(5.0f, 0.0f, -10.0f))
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof TransformMatrix, TransformMatrix, GL_DYNAMIC_DRAW);
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0, 2);
	
}

void MyGlWindow::mouseMoveEvent(QMouseEvent* e) {
	camera.mouseUpdate(glm::vec2(e->x(),e->y()));
	repaint();
}

//void MyGlWindow::keyPressEvent(QKeyEvent* e) {
//	qDebug() << e->key();
//	switch (e->key()) {
//	case Qt::Key::Key_W:
//		camera.movePosition(MOVE_TOWARD);
//		break;
//	case Qt::Key::Key_S:
//		camera.movePosition(MOVE_FORWARD);
//		break;
//	case Qt::Key::Key_A:
//		camera.movePosition(MOVE_LEFT);
//		break;
//	case Qt::Key::Key_D:
//		camera.movePosition(MOVE_RIGHT);
//		break;
//	case Qt::Key::Key_R:
//		camera.movePosition(MOVE_UP);
//		break;
//	case Qt::Key::Key_F:
//		camera.movePosition(MOVE_DOWN);
//		break;
//	}
//	repaint();
//}
void MyGlWindow::keyPressEvent() {
	if (GetAsyncKeyState('W'))
		camera.movePosition(MOVE_TOWARD);
	if (GetAsyncKeyState('S'))
		camera.movePosition(MOVE_FORWARD);
	if (GetAsyncKeyState('A'))
		camera.movePosition(MOVE_LEFT);
	if (GetAsyncKeyState('D'))
		camera.movePosition(MOVE_RIGHT);
	if (GetAsyncKeyState(VK_SPACE))
		camera.movePosition(MOVE_UP);
	if (GetAsyncKeyState(VK_SHIFT))
		camera.movePosition(MOVE_DOWN);
}
MyGlWindow::~MyGlWindow() {
	glUseProgram(0);
	glDeleteProgram(programID);
}