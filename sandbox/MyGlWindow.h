#ifndef SANDBOX_MY_GL_WINDOW
#define SANDBOX_MY_GL_WINDOW

#include <QtOpenGL/qgl.h>
#include <QtCore/qtimer.h>
#include <Time/Clock.h>
#include <Math/Vector2D.h>
#include <Primitives/Camera.h>

using Time::Clock;

class MyGlWindow : public QGLWidget
{
	Q_OBJECT

	QTimer myTimer;
	Clock myClock;
	Camera camera;
protected:
	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	//void keyPressEvent(QKeyEvent*)
public:
	~MyGlWindow();
	void setupVertexArray();
	void sendDataToOpenGL();
	bool checkStatus(
		GLuint objectID,
		PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
		PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
		GLenum statusType
	);
	bool checkShaderStatus(GLuint shaderID);
	bool checkProgramStatus(GLuint programID);
	std::string readShaderCode(const char* fileName);
	void installShaders();
	void keyPressEvent();
	//void myInitialize();
private slots:
	void myUpdate();
};

#endif