#include "objloader.h"
#include "fonctionsOpenGL.h"

// Cr�ation instance pour affichage OpenGL
FonctionsOpenGL * openGL; 

// Red�finition des m�thodes pour permettre � GlutDisplayFunc d'appeler les m�thodes de openGL
void resize (int p_width, int p_height)
{
	openGL->resize(p_width, p_height);
}
void keyboard (unsigned char key, int x, int y) 
{
	openGL->keyboard(key, x, y);
}
void mouseMovement(int x, int y) 
{
	openGL->mouseMovement(x, y);
}
void display(void)
{   
	openGL->display();
}
void vIdle()
{
	openGL->vIdle();
}

/** La fonction main.
* @param argv : listing des param�tres n�cessaire : l'obj � charger, la vid�o ou live, la configuration de la planche, les donn�es de corrections de d�formations de la cam�ra et la taille des marqueurs (en m)
*
*/
int main(int argc, char **argv)
{
	// Pour d�bugger plus vite !
	if (argc!=6) {
      cerr<<"Nombre d'arguments invalide"<<endl;
      cerr<<"Ordre demand� : objet (triangulaire) .obj (in.avi|live) boardConfig.yml  intrinsicsLogitech.yml   size "<<endl;
      return false;
   }
	

	// initialisation de l'instance avec les arguments pass�s � la fonction main
	openGL = new FonctionsOpenGL(argv[2],argv[3],argv[4],atof(argv[5]));

	// On utilise GLUT pour charger la fen�tre, g�rer les entr�es (clavier/souris) et interraction avec la fen�tre
	glutInit(&argc, argv);    
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(openGL->getWidth(),openGL->getHeight());    
	glutInitWindowPosition(0,0);
	glutCreateWindow("ObjetEtTracking");
	glutDisplayFunc(display);
	glutIdleFunc(vIdle);
	glutReshapeFunc (resize);
   



	// Prise en charge du clavier et souris 
	//glutPassiveMotionFunc(mouseMovement); 
	glutKeyboardFunc (keyboard); 

	//initialisation de l'obj
	openGL->initialisation(argv[1]);
	glutMainLoop();
	
    return(0);    
}