#ifndef OPGL_H
#define OPGL_H

#include <QMainWindow>
#include <QtOpenGL/qgl.h>
#include <GL/glu.h>

const GLuint num = 50;
typedef struct
{
  int r, g, b;
  GLfloat dist;
  GLfloat angle;
}stars;

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
    void timerEvent( QTimerEvent * );

    bool fullscreen;
    GLfloat xRot, yRot, zRot;
    GLfloat zoom;
    GLfloat tilt;
    GLfloat spin;
    GLuint loop;
    GLuint texture[1];

    bool twinkle;

    stars star[num];
};

#endif // OPGL_H
