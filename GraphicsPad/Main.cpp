#include <QtWidgets\qapplication.h>
#include <meGlWindow.h>
int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	meGlWindow meWindow;
	meWindow.show();
	meWindow.setWindowState(Qt::WindowFullScreen);
	return app.exec();
}