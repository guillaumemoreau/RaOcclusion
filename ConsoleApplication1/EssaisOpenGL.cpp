#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "stdafx.h"

using namespace std;


// Prototypes des fonctions implémentées
void InitGL();
void Draw();
void Reshape(int w, int h);



int main(int argc, char** argv)
{
   //initialisation du FreeGlut
   glutInit(&argc, argv);

   // différencier les mode RGBA et couleur indexé, 
   // permet aussi de choisir le "double-buffering" et les buffers à utiliser (depth, stencil, accumulation)
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
  
   //création fenetre de taille donnée
   glutInitWindowSize (800,600);
   glutCreateWindow ("EssaisOpenGL");
   //glutFullScreen();
   
   // pour définir les callbacks relatif aux actions de 
   // - affichage 
   // - redimensionnement
   // - interaction clavier/souris
   glutDisplayFunc (Draw);
   glutReshapeFunc (Reshape);
   /*
   glutIgnoreKeyRepeat(1);
   glutKeyboardFunc(keyboard);
   glutKeyboardUpFunc(keyboardup);
   glutSpecialFunc(special);
   glutSpecialUpFunc(specialup) ;
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(motion);
   */

   //callback lancé quand le système n'a plus rien à faire => permet création d'animations 
   /*
   glutIdleFunc(void (*func)(void))
   */

   // Initialiser OpenGL
   InitGL();

   // boucle infinie => derniere ligne du programme pour permettre affichage en continu
   glutMainLoop();

   return 0;
}



/** 
Fonction d'affichage
*/
void Draw()             
{
	//Vide les buffers en param
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	//Choisit la matrice MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	//Réinitialise la matrice
	glLoadIdentity(); 	

	// Transformation de la vue (UTILISER CAMERA ARUCO)
	// http://www.opengl.org/sdk/docs/man2/xhtml/gluLookAt.xml
	gluLookAt(0,0,-10,0,0,0,0,1,0);
	
	// Tracer une figure (ENFIN!!!!!!!!!!)
	/* glBegin(GL_TRIANGLES);
		glVertex2i(0,1);
		glVertex2i(-1,0);
		glVertex2i(1,0);
	glEnd(); 
	*/

	
	glEnableClientState(GL_VERTEX_ARRAY);
	float *vertices, *normals, *textures, *colours;
	glVertexPointer(3,GL_FLOAT,0,vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	//Echange les 2 frame buffers (celui qui est affiché et celui qu'on remplit en mémoire)
	glutSwapBuffers(); //Attention : pas SwapBuffers(DC), utiliser pour fênetre Windows native !

	//Recalculer la scène
	glutPostRedisplay(); 

}



/**
Callback si fenetre redimentionnée
Fonction utilisée au moins une fois : à l'ouverture de la fenetre
*/
void Reshape(int w, int h)             
{
            // hauteur non null (pour éviter division par zéro)
            if (h == 0) h = 1; 

			//mise à jour du Viewport
            glViewport(0,0,w,h);

			// chargement matrice de projection 
            glMatrixMode(GL_PROJECTION);
            // définition comme matrice identité
			glLoadIdentity();
           
			/* gluPerspective modifie la matrice courante (c'est-à-dire ici la matrice de projection) 
			pour qu'OpenGL transforme les coordonnées 3D au moment du rendu en coordonnées 2D par rapport à l'écran,
			de façon à ce qu'on ait l'impression de regarder à travers un objectif de 45 de focale, de largeur width et de hauteur height, 
			et dont le clipping va de 0.1 à 100 (attention la valeur du clipping near - ici 0.1 - doit être strictement supérieure à 0)
			*/
			gluPerspective(
			45,
			float(w)/float(h),
			0.1,
			100
			); 
				
			// chargement matrice de positionnement de la vue (optionnel)
            glMatrixMode(GL_MODELVIEW);
			// définition comme matrice identité
			glLoadIdentity();

}


/** 
Fonction d'initialisation d'OpenGL
*/
void InitGL() 
{
}


/**
Fonction utiliser par le loader
*/
string doubleSlash(string s)
{
    //Remplace "//" par "/1/".
    string s1="";
    for(unsigned int i=0;i<s.size();i++)
    {
        if(i<s.size()-1&&s[i]=='/'&&s[i+1]=='/')
        {
            s1+="/1/";
            i++;
        }
        else
            s1+=s[i];
    }
    return s1;
}

string remplacerSlash(string s)
{
    //Remplace les '/' par des espaces.
    string ret="";
    for(unsigned int i=0;i<s.size();i++)
    {
        if(s[i]=='/')
            ret+=' ';
        else
            ret+=s[i];
    }
    return ret;
}

vector<string> splitSpace(string s)
{
    //Eclate une chaîne au niveau de ses espaces.
    vector<string> ret;
    string s1="";
    for(unsigned int i=0;i<s.size();i++)
    {
        if(s[i]==' '||i==s.size()-1)
        {
            if(i==s.size()-1)
                s1+=s[i];
            ret.push_back(s1);
            s1="";
        }
        else
            s1+=s[i];
    }
    return ret;
}

float* vector2float(vector<float>& tableau)
{
    float* t=NULL;
    t=(float*)malloc(tableau.size()*sizeof(float));
    if(t==NULL||tableau.empty())
    {
        float *t1=(float*)malloc(sizeof(float)*3);
        for(int i=0;i<3;i++)
            t1[i]=0.;
        return t1;
    }

    for(unsigned int i=0;i<tableau.size();i++)
        t[i]=tableau[i];
    return t;
}
