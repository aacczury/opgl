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

float colours[] = {
  1.0f, 0.0f,  0.0f,
  0.0f, 1.0f,  0.0f,
  0.0f, 0.0f,  1.0f
};

void opgl::initializeGL()
{
    initializeGLFunctions();// important

    glGenBuffers (1, &points_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
    glBufferData (GL_ARRAY_BUFFER, 3 * 3 * sizeof (float), points, GL_STATIC_DRAW);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    glGenBuffers (1, &colours_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
    glBufferData (GL_ARRAY_BUFFER, 3 * 3 * sizeof (float), colours, GL_STATIC_DRAW);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    viewMatrix.setRow(0, QVector4D(1.0f, 0.0f, 0.0f, 0.5f)); // x + 0.5
    viewMatrix.setRow(1, QVector4D(0.0f, 1.0f, 0.0f, 0.0f));
    viewMatrix.setRow(2, QVector4D(0.0f, 0.0f, 1.0f, 0.0f));
    viewMatrix.setRow(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));

    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    const char *vsrc =
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_colour;"

        "uniform mat4 matrix;"

        "out vec3 colour;"

        "void main () {"
            "colour = vertex_colour;"
            "gl_Position = matrix * vec4 (vertex_position, 1.0);"
        "}";
    vshader->compileSourceCode(vsrc); //glShaderSource && glCompileShader

    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
    const char *fsrc =
        "in vec3 colour;"
        "out vec4 frag_colour;\n"

        "void main () {\n"
          "frag_colour = vec4 (colour, 1.0);\n"
        "}\n";
    fshader->compileSourceCode(fsrc);//glShaderSource && glCompileShader

    shader_programme.addShader(vshader); //glAttachShader
    shader_programme.addShader(fshader); //glAttachShader
    shader_programme.bindAttributeLocation("vertex_position", 0); //沒有好像也沒差
    shader_programme.bindAttributeLocation("vertex_colour", 1);
    shader_programme.link(); //glLinkProgram*/
}

void opgl::resizeGL(int w, int h){
}

void opgl::paintGL(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_programme.bind();

    glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(shader_programme.attributeLocation("vertex_position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shader_programme.attributeLocation("vertex_position"));

    glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
    glVertexAttribPointer(shader_programme.attributeLocation("vertex_colour"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shader_programme.attributeLocation("vertex_colour"));

    shader_programme.setUniformValue(shader_programme.uniformLocation("matrix"), viewMatrix);

    glDrawArrays (GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(shader_programme.attributeLocation("vertex_position"));
    glDisableVertexAttribArray(shader_programme.attributeLocation("vertex_colour"));
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
