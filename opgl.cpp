#include "opgl.h"
#include <math.h>

static GLfloat boxcol[5][3] =
{
  { 1.0, 0.0, 0.0 },
  { 1.0, 0.5, 0.0 },
  { 1.0, 1.0, 0.0 },
  { 0.0, 1.0, 0.0 },
  { 0.0, 1.0, 1.0 }
};

static GLfloat topcol[5][3] =
{
  { 0.5, 0.0, 0.0 },
  { 0.5, 0.25, 0.0 },
  { 0.5, 0.5, 0.0 },
  { 0.0, 0.5, 0.0 },
  { 0.0, 0.5, 0.5 }
};


opgl::opgl( QWidget *parent, bool fs ) :
    QGLWidget( parent ){
    xRot = yRot = zRot = 0.0;
    box = top = 0;

    xLoop = yLoop = 0;

    fullscreen = fs;
    setGeometry( 400, 200, 800, 600 );

    if( fullscreen )
        showFullScreen();
}

void opgl::initializeGL(){
    loadGLTextures();
    buildLists();

    glEnable( GL_TEXTURE_2D );
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0, 0.0, 0.0, 0.5 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_LIGHT0 );
    glEnable( GL_LIGHTING );
    glEnable( GL_COLOR_MATERIAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void opgl::paintGL(){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindTexture( GL_TEXTURE_2D, texture[0] );

    for ( yLoop = 1; yLoop < 6; yLoop++ ){
        for ( xLoop = 0; xLoop < yLoop; xLoop++ ){
            glLoadIdentity();
            glTranslatef( 1.4 + (float(xLoop) * 2.8) - (float(yLoop) * 1.4),
                ( (6.0 - (float(yLoop)) ) * 2.4 ) - 7.0, -20.0 );
            glRotatef( 45.0 - (2.0 * yLoop) + xRot, 1.0, 0.0, 0.0 );
            glRotatef( 45.0 + yRot, 0.0, 1.0, 0.0 );
            glColor3fv( boxcol[yLoop-1] );
            glCallList( box );
            glColor3fv( topcol[yLoop-1] );
            glCallList( top );
        }
    }
}

void opgl::loadGLTextures(){
    QImage tex, buf;
    if( !buf.load( "frozen.bmp" ) ){
        QImage dummy( 128, 128, QImage::Format_RGB888 );
        dummy.fill( Qt::green );
        buf = dummy;
    }
    tex = QGLWidget::convertToGLFormat( buf );

    glGenTextures( 1, &texture[0] );

    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
}

void opgl::resizeGL(int w, int h){
    if( h == 0 )
        h = 1;
    glEnable( GL_BLEND );
    glDisable( GL_DEPTH_TEST );
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, w/h, 0.1, 100.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void opgl::buildLists(){
    box = glGenLists( 2 );
    glNewList( box, GL_COMPILE );

    glBegin( GL_QUADS );
    glNormal3f( 0.0, -1.0, 0.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0, -1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );

    glNormal3f( 0.0, 0.0, 1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );

    glNormal3f( 0.0, 0.0, -1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );

    glNormal3f( 1.0, 0.0, 0.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );

    glNormal3f( -1.0, 0.0, 0.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
    glEnd();
    glEndList();

    top = box + 1;
    glNewList( top, GL_COMPILE );
    glBegin( GL_QUADS );
    glNormal3f( 0.0, 1.0, 0.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
    glEnd();
    glEndList();
}

opgl::~opgl(){
}
