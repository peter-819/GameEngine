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

#include <Windows.h>

GLuint programID;
GLuint LightProgramID;

GLuint PlaneDrawNumVertices;
GLuint PlaneVAOID;
GLuint PlanevertexBufferID;
GLuint PlaneindexBufferID;

GLuint CubeDrawNumVertices;
GLuint CubeVAOID;
GLuint CubevertexBufferID;
GLuint CubeindexBufferID;

GLuint TransformMatrixUniformLocation;
GLuint FullMatrixUniformLocation;
GLuint LightPositionUniformLocation;
GLuint AmbientLightUniformLocation;
GLuint CameraPositionUniformLocation;

GLuint MouseTracking = 1;

using glm::vec3;
using glm::mat4;
vec3 LightPosition = vec3(0.0f, 5.0f, 0.0f);
float LightMoveSpeed = 0.1f;

mat4 CubeRandomTransform[10];

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

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = readShaderCode(".\\Shaders\\LightBallVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode(".\\Shaders\\LightBallFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);
	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID)) {
		return;
	}
	LightProgramID = glCreateProgram();
	glAttachShader(LightProgramID, vertexShaderID);
	glAttachShader(LightProgramID, fragmentShaderID);
	glLinkProgram(LightProgramID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	if (!checkProgramStatus(LightProgramID)) {
		return;
	}
}

void MyGlWindow::setupVertexArray() {
	glGenVertexArrays(1, &PlaneVAOID);
	glGenVertexArrays(1, &CubeVAOID);

	glBindVertexArray(PlaneVAOID);
	glBindBuffer(GL_ARRAY_BUFFER, PlanevertexBufferID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PlaneindexBufferID);

	glBindVertexArray(CubeVAOID);
	glBindBuffer(GL_ARRAY_BUFFER, CubevertexBufferID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeindexBufferID);

}

void MyGlWindow::sendDataToOpenGL() {
	ShapeData plane = ShapeGenerator::makePlane(25);
	PlaneDrawNumVertices = plane.NumIndices;

	glGenBuffers(1, &PlanevertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, PlanevertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, plane.vertexBufferSize(), plane.Vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &PlaneindexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PlaneindexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane.indexBufferSize(), plane.Indices , GL_STATIC_DRAW);
	
	ShapeData cube = ShapeGenerator::makeCube();
	CubeDrawNumVertices = cube.NumIndices;

	glGenBuffers(1, &CubevertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, CubevertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, cube.vertexBufferSize(), cube.Vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &CubeindexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeindexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indexBufferSize(), cube.Indices, GL_STATIC_DRAW);

	for (int i = 1; i < 10; i++) {
		CubeRandomTransform[i] =
			glm::translate(vec3(rand() % 10 - 4, rand() % 3, rand() % 10 - 5))
			* glm::scale(vec3(0.5, 0.5, 0.5));
	}
	plane.cleanup();
	cube.cleanup();
}

void MyGlWindow::initializeGL() {
	setCursor(Qt::BlankCursor);
	setMouseTracking(true);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	sendDataToOpenGL();
	installShaders();
	setupVertexArray();
	
	int midx = this->x() + this->height() / 2;
	int midy = this->y() + this->width() / 2;
	SetCursorPos(midx, midy);

	TransformMatrixUniformLocation = glGetUniformLocation(programID, "TransformMatrix");
	FullMatrixUniformLocation = glGetUniformLocation(programID, "FullMatrix");
	LightPositionUniformLocation = glGetUniformLocation(programID, "LightPosition");
	AmbientLightUniformLocation = glGetUniformLocation(programID, "AmbientLight");
	CameraPositionUniformLocation = glGetUniformLocation(programID, "CameraPosition");
	connect(&myTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
	myTimer.start(17);  //60fps
}


void MyGlWindow::paintGL() {
	glClear(GL_DEPTH_BUFFER_BIT| GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	glUseProgram(programID);

	mat4 projectionMatrix = mat4(1.0f);
	mat4 cameraMatrix = mat4(1.0f);
	mat4 TransformMatrix = mat4(1.0f);
	mat4 FullMatrix = mat4(1.0f);
	projectionMatrix = glm::perspective(glm::radians(60.0f),
		static_cast<float>(width()) / height(), 0.1f, 50.0f);
	cameraMatrix = camera.getWorldToViewMatrix();

	vec3 cameraPosition = camera.getPosition();
	glUniform3fv(LightPositionUniformLocation, 1, &LightPosition[0]);
	glUniform4f(AmbientLightUniformLocation, 0.15f, 0.15f, 0.15f, 1.0f);
	glUniform3fv(CameraPositionUniformLocation, 1, &cameraPosition[0]);
	//Draw Plane
	glUseProgram(programID);
	FullMatrix = projectionMatrix * cameraMatrix * TransformMatrix;			
	glUniformMatrix4fv(TransformMatrixUniformLocation, 1, GL_FALSE, &TransformMatrix[0][0]);
	glUniformMatrix4fv(FullMatrixUniformLocation, 1, GL_FALSE, &FullMatrix[0][0]);
	glBindVertexArray(PlaneVAOID);
	glDrawElements(GL_TRIANGLES, PlaneDrawNumVertices, GL_UNSIGNED_SHORT, 0);
	//Draw Cube
	//TransformMatrix = glm::translate(vec3(-4.0f, 2.0f, -4.0f));
	//FullMatrix = projectionMatrix * cameraMatrix * TransformMatrix;
	//glUniformMatrix4fv(TransformMatrixUniformLocation, 1, GL_FALSE, &TransformMatrix[0][0]);
	//glUniformMatrix4fv(FullMatrixUniformLocation, 1, GL_FALSE, &FullMatrix[0][0]);
	glBindVertexArray(CubeVAOID);
	//glDrawElements(GL_TRIANGLES, CubeDrawNumVertices, GL_UNSIGNED_SHORT, 0);
	for (int i = 1; i < 10; i++) {
		TransformMatrix = CubeRandomTransform[i];
		FullMatrix = projectionMatrix * cameraMatrix * TransformMatrix;
		glUniformMatrix4fv(TransformMatrixUniformLocation, 1, GL_FALSE, &TransformMatrix[0][0]);
		glUniformMatrix4fv(FullMatrixUniformLocation, 1, GL_FALSE, &FullMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, CubeDrawNumVertices, GL_UNSIGNED_SHORT, 0);
	}

	TransformMatrix = glm::translate(LightPosition) * glm::scale(vec3(0.2f, 0.2f, 0.2f));
	FullMatrix = projectionMatrix * cameraMatrix * TransformMatrix;
	GLuint lightFullMatrixUL = glGetUniformLocation(LightProgramID, "fullMatrix");
	glUseProgram(LightProgramID); 
	glUniformMatrix4fv(lightFullMatrixUL, 1, GL_FALSE, &FullMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, CubeDrawNumVertices, GL_UNSIGNED_SHORT, 0);
}

//void MyGlWindow::mouseMoveEvent(QMouseEvent* e) {
//	camera.mouseUpdate(glm::vec2(e->x(),e->y()));
//	repaint();
//}

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
	if (GetAsyncKeyState('P'))
		this->showFullScreen();
	if (GetAsyncKeyState(VK_ESCAPE))
		this->close();
	if (GetAsyncKeyState('M'))
		MouseTracking ^= 1;

	if (GetAsyncKeyState(VK_UP))
		LightPosition.z -= LightMoveSpeed;
	if (GetAsyncKeyState(VK_DOWN))
		LightPosition.z += LightMoveSpeed;
	if (GetAsyncKeyState(VK_LEFT))
		LightPosition.x -= LightMoveSpeed;
	if (GetAsyncKeyState(VK_RIGHT))
		LightPosition.x += LightMoveSpeed;
	if (GetAsyncKeyState('J'))
		LightPosition.y += LightMoveSpeed;
	if (GetAsyncKeyState('K'))
		LightPosition.y -= LightMoveSpeed;
}

int ABS(const int& a) { return a > 0 ? a : -a; }

void MyGlWindow::myMouseMove() {
	POINT p;
	GetCursorPos(&p);
	int midx = this->x() + this->height()/2;
	int midy = this->y() + this->width()/2;
	//qDebug() << this->x() << this->y() << this->height() << this->width();
	camera.mouseUpdate(glm::vec2(p.x - midx, p.y - midy));
	SetCursorPos(midx, midy);
}

void MyGlWindow::myUpdate() {
	myClock.clockGetNewFrame();
	float frameTime = myClock.clockTimeLastFrame();
	keyPressEvent();
	if(MouseTracking) myMouseMove();
	repaint();
}

MyGlWindow::~MyGlWindow() {
	glUseProgram(0);
	glDeleteProgram(programID);
	glDeleteProgram(LightProgramID);
}