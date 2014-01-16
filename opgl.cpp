#include "opgl.h"

GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightPosition[4] = { 0.0, 0.0, 2.0, 1.0 };

opgl::opgl( QWidget *parent, bool fs ) :
    QGLWidget( parent ){
    xRot = yRot = zRot = 0.0;
    zoom = -15.0;
    tilt = 90.0;
    spin = 0.0;
    loop = 0;

    twinkle = true;

    fullscreen = fs;
    setGeometry( 400, 200, 800, 600 );

    if( fullscreen )
        showFullScreen();

    startTimer( 5 );
}

void opgl::initializeGL(){
    loadGLTextures();
    glEnable( GL_TEXTURE_2D );
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0, 0.0, 0.0, 0.5 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    glEnable( GL_BLEND );
    for ( loop = 0; loop < num; loop++ )
    {
      star[loop].angle = 0.0;
      star[loop].dist = ( float(loop)/num ) * 5.0;
      star[loop].r = rand() % 256;
      star[loop].g = rand() % 256;
      star[loop].b = rand() % 256;
    }
}

void opgl::paintGL(){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    for ( loop = 0; loop < num; loop++ ){
        glLoadIdentity();
        glTranslatef(  0.0,  0.0, zoom );
        glRotatef( tilt, 1.0, 0.0, 0.0 );
        glRotatef( star[loop].angle, 0.0, 1.0, 0.0 );
        glTranslatef( star[loop].dist, 0.0, 0.0 );
        glRotatef( -star[loop].angle, 0.0, 1.0, 0.0 );
        glRotatef( -tilt, 1.0, 0.0, 0.0 );

        if ( twinkle )
        {
          glColor4ub( star[(num-loop)-1].r,
             star[(num-loop)-1].g,
             star[(num-loop)-1].b, 255 );
          glBegin( GL_QUADS );
            glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, 0.0 );
            glTexCoord2f( 1.0, 0.0 ); glVertex3f( 1.0, -1.0, 0.0 );
            glTexCoord2f( 1.0, 1.0 ); glVertex3f( 1.0, 1.0, 0.0 );
            glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0, 1.0, 0.0 );
          glEnd();
        }

        glRotatef( spin, 0.0, 0.0, 1.0 );
        glColor4ub( star[loop].r, star[loop].g, star[loop].b, 255 );
        glBegin( GL_QUADS );
          glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, 0.0 );
          glTexCoord2f( 1.0, 0.0 ); glVertex3f( 1.0, -1.0, 0.0 );
          glTexCoord2f( 1.0, 1.0 ); glVertex3f( 1.0, 1.0, 0.0 );
          glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0, 1.0, 0.0 );
        glEnd();

        spin += 0.01;
        star[loop].angle += float(loop)/num;
        star[loop].dist -= 0.01;

        if ( star[loop].dist < 0.0 )
        {
          star[loop].dist += 5.0;
          star[loop].r = rand() % 256;
          star[loop].g = rand() % 256;
          star[loop].b = rand() % 256;
        }
    }
}

void opgl::loadGLTextures(){
    QImage tex, buf;
    if( !buf.load( "star.bmp" ) ){
        QImage dummy( 128, 128, QImage::Format_RGB888 );
        dummy.fill( Qt::green );
        buf = dummy;
    }
    tex = QGLWidget::convertToGLFormat( buf );

    glGenTextures( 1, &texture[0] );

    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
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

void opgl::timerEvent(QTimerEvent*){
  updateGL();
}

opgl::~opgl(){
}
