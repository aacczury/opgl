#ifndef OPGL_H
#define OPGL_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <QGLWidget>
#include <QGLShaderProgram>
#include <QtOpenGL>
#include <QGLFunctions>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QTime>
#include <QTimer>

class opgl : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    explicit opgl(QGLWidget *parent = 0);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    void loadCubeTexture();
    void drawTriangle(const QVector<QVector4D> &colors);

    unsigned int points_vbo;
    unsigned int colours_vbo;

    QGLShaderProgram shader_programme;
    int mModelHandle;
    int mViewProjHandle;
    int mCameraHandle;
    int mWorldHandle;

    QMatrix4x4 mModelMatrix; //模型矩陣
    QMatrix4x4 mViewProjMatrix; //視圖投影矩陣
    QVector3D mCameraV3; //相機位置

    GLuint tex_cube;
};

#endif // OPGL_H
