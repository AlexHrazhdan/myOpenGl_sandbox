#pragma once
#include <QtWidgets\qopenglwidget.h>

class meGlWindow: public QOpenGLWidget
{
protected:
	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void setupVertexArrays();
	void sendDataToOpenGL();
public:
	meGlWindow();
	~meGlWindow();

	
};