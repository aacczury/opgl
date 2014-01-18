#ifndef OPGL_H
#define OPGL_H

#include <QMainWindow>
#include <QtOpenGL/qgl.h>
#include <GL/glu.h>

class opgl : public QGLWidget
{
    Q_OBJECT

public:
    explicit opgl( QWidget *parent = 0, bool fs = false );
    ~opgl();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void loadGLTextures();

    bool fullscreen;

    GLfloat xRot, yRot, zRot;
    GLfloat zoom;
    GLfloat xSpeed, ySpeed;
    GLuint texture[3];
    GLuint filter;

    bool light;

    GLuint fogFilter;
};

#endif // OPGL_H
