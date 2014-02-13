#include "objloader.h"
#include "fonctionsOpenGL.h"


/** La fonction main.
* @param argv : listing des paramètres nécessaire : l'obj à charger, la vidéo ou live, la configuration de la planche, les données de corrections de déformations de la caméra et la taille des marqueurs (en m)
*
*/
int main(int argc, char **argv)
{
	/** Pour débugger plus vite !
	if (a/rgc!=6) {
        cerr<<"Nombre d'arguments invalide"<<endl;
        cerr<<"Ordre demandé : objet (triagulaire) .obj (in.avi|live) boardConfig.yml  intrinsicsLogitech.yml   size "<<endl;
        return false;
    }
	*/

	FonctionsOpenGL * openGL = new FonctionsOpenGL(argv[2],argv[3],argv[4],atof(argv[5]));

	/*
	// On utilise GLUT pour charger la fenêtre, gérer les entrées (clavier/souris) et interraction avec la fenêtre
	glutInit(&argc, argv);    
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(openGL.getWidth(),openGL.getHeight());    
	glutInitWindowPosition(0,0);
	glutCreateWindow("ObjetEtTracking");
	glutDisplayFunc(openGL->*display);
	glutIdleFunc(openGL->*vIdle);
	glutReshapeFunc (openGL->*resize);

	// Prise en charge du clavier et souris 
	//glutPassiveMotionFunc(mouseMovement); 
	glutKeyboardFunc (openGL->*keyboard); 

	//initialisation de l'obj
	openGL->initialisation(argv[1]);
	glutMainLoop();
	*/
    return(0);    
}