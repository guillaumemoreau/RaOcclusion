#include "GL\glut.h" 

void axis(float size)
{
    glColor3f (1,0,0 );
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
    glVertex3f(size,0.0f, 0.0f); // ending point of the line
    glEnd( );

    glColor3f ( 0,1,0 );
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
    glVertex3f( 0.0f,size, 0.0f); // ending point of the line
    glEnd( );


    glColor3f (0,0,1 );
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
    glVertex3f(0.0f, 0.0f, size); // ending point of the line
    glEnd( );


}
