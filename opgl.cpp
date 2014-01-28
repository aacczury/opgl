#include "opgl.h"
#include <math.h>

opgl::opgl(QGLWidget *parent) :
    QGLWidget(parent)
{
    setGeometry( 400, 200, 800, 600 );
}

float points[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f

};

void opgl::initializeGL()
{
    initializeGLFunctions();// important

    glEnable(GL_TEXTURE_CUBE_MAP);
    loadCubeTexture();

    glGenBuffers (1, &points_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
    glBufferData (GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), points, GL_STATIC_DRAW);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    projMatrix.setRow(0, QVector4D(0.5f, 0.0f, 0.0f, 0.0f));  // 1/r
    projMatrix.setRow(1, QVector4D(0.0f, 0.5f, 0.0f, 0.0f));  // 1/t
    projMatrix.setRow(2, QVector4D(0.0f, 0.0f, 1.0f, 0.0f));  // -2/(f-n) // -(f+n)/(f-n)
    projMatrix.setRow(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));

    viewMatrix.setRow(0, QVector4D(1.0f, 0.0f, 0.0f, 0.5f)); // x + 0.5
    viewMatrix.setRow(1, QVector4D(0.0f, 1.0f, 0.0f, 0.0f));
    viewMatrix.setRow(2, QVector4D(0.0f, 0.0f, 1.0f, 0.0f));
    viewMatrix.setRow(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));


    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    const char *vsrc =
        "in vec3 vp;"

        "uniform mat4 proj, view;"
        "out vec3 texcoords;"

        "void main () {"
            "texcoords = vp;"
            "gl_Position = proj * view * vec4 (vp, 1.0);"
        "}";
    vshader->compileSourceCode(vsrc); //glShaderSource && glCompileShader

    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
    const char *fsrc =
        "in vec3 texcoords;"
        "uniform samplerCube cube_texture;"
        "out vec4 frag_colour;"

        "void main () {"
            "frag_colour = texture (cube_texture, texcoords);"
        "}";
    fshader->compileSourceCode(fsrc);//glShaderSource && glCompileShader

    shader_programme.addShader(vshader); //glAttachShader
    shader_programme.addShader(fshader); //glAttachShader
    shader_programme.link(); //glLinkProgram*/
}

void opgl::resizeGL(int w, int h){
}

void opgl::paintGL(){
    glDepthMask (GL_FALSE);
    shader_programme.bind();

    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_CUBE_MAP, tex_cube);

    glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(shader_programme.attributeLocation("vp"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shader_programme.attributeLocation("vp"));

    shader_programme.setUniformValue(shader_programme.uniformLocation("proj"), projMatrix);
    shader_programme.setUniformValue(shader_programme.uniformLocation("view"), viewMatrix);

    glDrawArrays (GL_TRIANGLES, 0, 36);

    glDisableVertexAttribArray(shader_programme.attributeLocation("vp"));

    glDepthMask (GL_TRUE);
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
