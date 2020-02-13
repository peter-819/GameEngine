#include <QtWidgets/qapplication.h>
#include <QtWidgets/qwidget.h>
#include "MyGlWindow.h"

int main(int argc, char* argv[]) {
	QApplication Application(argc, argv);
	MyGlWindow myGlWidget;
	myGlWidget.myInitialize();
	myGlWidget.show();
	return Application.exec();
}