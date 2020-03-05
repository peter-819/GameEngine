#include <QtWidgets/qapplication.h>
#include <QtWidgets/qwidget.h>

#include "MyGlWindow.h"

int main(int argc, char* argv[]) {
	QApplication Application(argc, argv);
	MyGlWindow myGlWidget;
	myGlWidget.setFixedSize(800, 800);
	myGlWidget.show();
	return Application.exec();
}