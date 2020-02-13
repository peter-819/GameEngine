#ifndef SANDBOX_MY_GL_WINDOW
#define SANDBOX_MY_GL_WINDOW

#include <QtOpenGL/qgl.h>
#include <QtCore/qtimer.h>
#include <Time/Clock.h>
#include <Math/Vector2D.h>

using Time::Clock;

class MyGlWindow : public QGLWidget
{
	Q_OBJECT

	QTimer myTimer;
	Clock myClock;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int, int);
public:
	void myInitialize();
private slots:
	void myUpdate();
	void myCheckKeyState();
};

#endif