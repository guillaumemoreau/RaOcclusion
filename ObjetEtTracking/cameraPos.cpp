#include "GL\glut.h" 

void axis(float size)
{
    glColor3f (1,0,0 );
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f); // origine de la ligne
    glVertex3f(size,0.0f, 0.0f); // fin de la ligne
    glEnd( );

    glColor3f ( 0,1,0 );
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f); 
    glVertex3f( 0.0f,size, 0.0f); 
    glEnd( );


    glColor3f (0,0,1 );
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f); 
    glVertex3f(0.0f, 0.0f, size); 
    glEnd( );


}
