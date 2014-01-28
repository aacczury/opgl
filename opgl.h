#ifndef OPGL_H
#define OPGL_H

#include <QMainWindow>
#include <QtOpenGL>
#include <QGLFunctions>
#include <GL/glu.h>
#include <iostream>
using namespace std;

class opgl : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

public:
    explicit opgl( QWidget *parent = 0, bool fs = false );
    ~opgl();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void loadCubeTexture();
    void loadGLTextures();

    bool fullscreen;

    GLfloat xRot, yRot, zRot;
    GLfloat zoom;
    GLfloat xSpeed, ySpeed;
    GLuint texture, ctex;
    GLuint filter;

    bool light;

    GLuint fogFilter;
};

#endif // OPGL_H
