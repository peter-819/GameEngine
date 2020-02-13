#include <GL/glew.h>
#include "MyGlWindow.h"
#include <cassert>
#include <QtCore/qdebug>
using Math::Vector2D;
namespace {
	Vector2D verts[] = {
			Vector2D(+0.1f,-0.1f),
			Vector2D(+0.0f,+0.1f),
			Vector2D(-0.1f,-0.1f),
	};
	Vector2D shipPosition(0.0f, 0.0f);
	Vector2D shipVelocity(0.0f, 0.0f);
	const int VERTS_NUM = sizeof(verts) / sizeof(*verts);
}

void MyGlWindow::initializeGL() {
	GLenum errorCode = glewInit();
	assert(errorCode == 0);
	GLuint myBufferID;
	glGenBuffers(1, &myBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, myBufferID);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof verts, NULL,
		GL_DYNAMIC_DRAW);

	connect(&myTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
	myTimer.start(17);  //60fps
}

void MyGlWindow::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBufferData(GL_ARRAY_BUFFER, sizeof verts, verts, GL_DYNAMIC_DRAW);

	Vector2D transVerts[VERTS_NUM];
	for (int i = 0; i < VERTS_NUM; i++)
		transVerts[i] = verts[i] + shipPosition;

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof transVerts, transVerts);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MyGlWindow::myUpdate() {
	myClock.clockGetNewFrame();
	float frameTime = myClock.clockTimeLastFrame();
	myCheckKeyState();
	shipPosition = shipPosition + shipVelocity;
	repaint();
}

void MyGlWindow::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
}

void MyGlWindow::myInitialize() {
	myClock.clockInitialize();
}

/*void MyGlWindow::myCheckKeyState() {
	const float SPEED = 1.0f * myClock.clockTimeLastFrame();
	qDebug() << SPEED;
	if (GetAsyncKeyState(VK_UP))
		shipPosition.y += SPEED;
	if (GetAsyncKeyState(VK_DOWN))
		shipPosition.y -= SPEED;
	if (GetAsyncKeyState(VK_RIGHT))
		shipPosition.x += SPEED;
	if (GetAsyncKeyState(VK_LEFT))
		shipPosition.x -= SPEED;
}*/

void MyGlWindow::myCheckKeyState() {
	const float ACCERALATION = 0.08f * myClock.clockTimeLastFrame();
	if (GetAsyncKeyState(VK_UP))
		shipVelocity.y += ACCERALATION;
	if (GetAsyncKeyState(VK_DOWN))
		shipVelocity.y -= ACCERALATION;
	if (GetAsyncKeyState(VK_RIGHT))
		shipVelocity.x += ACCERALATION;
	if (GetAsyncKeyState(VK_LEFT))
		shipVelocity.x -= ACCERALATION;
	if (GetAsyncKeyState(VK_SPACE))
		shipVelocity.x = 0, shipVelocity.y = 0;
}