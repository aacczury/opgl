#include "opgl.h"
#include <math.h>

opgl::opgl(QGLWidget *parent) :
    QGLWidget(parent)
{
    setGeometry( 400, 200, 800, 600 );
}

float points[] = {
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
};

void opgl::initializeGL()
{
    initializeGLFunctions();
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, 3 * 3 * sizeof (float), points, GL_STATIC_DRAW);

    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    const char *vsrc =
        "#version 400\n"

        "in vec3 vp;\n"

        "void main () {\n"
          "gl_Position = vec4 (vp, 1.0);\n"
        "}\n";
    vshader->compileSourceCode(vsrc); //glShaderSource && glCompileShader

    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
    const char *fsrc =
        "#version 400\n"

        "out vec4 frag_colour;\n"

        "void main () {\n"
          "frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);\n"
        "}\n";
    fshader->compileSourceCode(fsrc);//glShaderSource && glCompileShader

    shader_programme.addShader(vshader); //glAttachShader
    shader_programme.addShader(fshader); //glAttachShader
    shader_programme.link(); //glLinkProgram*/
}

void opgl::resizeGL(int w, int h){
}

void opgl::paintGL(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_programme.bind();

    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glDrawArrays (GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
}

GLenum cubesides[6] = { // faces of cube texture
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

void opgl::loadCubeTexture(){

    glGenTextures( 1, &tex_cube );
    glBindTexture( GL_TEXTURE_CUBE_MAP, tex_cube );
    for(unsigned int i=0;i<6;i++){
        QImage tex, buf;
        QString filename = QString::number(i);
        filename.append(".bmp");
        if( !buf.load( filename ) ){
            QImage dummy( 128, 128, QImage::Format_RGB888 );
            dummy.fill( Qt::green );
            buf = dummy;
        }
        tex = QGLWidget::convertToGLFormat( buf );

        glTexImage2D(cubesides[i],0,GL_RGB,tex.width(),tex.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex.bits());
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
}
