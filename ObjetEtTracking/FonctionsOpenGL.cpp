#include "FonctionsOpenGL.h"

/** Constructeur permettant le lancement de l'affichage d'OpenGL.
Ce constructeur fait appel � Aruco et OpenCV pour d�tecter les markers sur la vid�o. 
Il affiche ensuite les fichiers obj correctement. */
FonctionsOpenGL::FonctionsOpenGL(string TheInputVideo, string TheBoardConfigFile, string TheIntrinsicFile, float TheMarkerSize) 
{
	/* D�pendance � Aruco et OpenCv */
	//vid�o
	this->TheInputVideo=TheInputVideo;
	//Fichier config du board
	this->TheBoardConfigFile=TheBoardConfigFile;
	//Cam�ra calibration info
	this->TheIntrinsicFile=TheIntrinsicFile;
	//taille des marqueurs
	this->TheMarkerSize=TheMarkerSize;

	//Lecture des infos du board
	TheBoardConfig.readFromFile(TheBoardConfigFile);

	//Charge la vid�o donn�e en param�tre
	if (TheInputVideo=="live")  //Lecture depuis la premi�re webcam sur l'ordinateur
		TheVideoCapturer.open(0);
	else TheVideoCapturer.open(TheInputVideo);
	if (!TheVideoCapturer.isOpened())
	{
		cerr<<"Impossible de lire la video"<<endl;
	}
       
	//Lecture de la premi�re image
	TheVideoCapturer>>TheInputImage;
	//Lit les param�tres de la cam�ra (pass�s en argument)
	TheCameraParams.readFromXMLFile(TheIntrinsicFile);
	TheCameraParams.resize(TheInputImage.size());

	//Taille de la fenetre adapt� � la vid�o / webcam
	screen_width = TheInputImage.size().width;
	screen_height = TheInputImage.size().height;

	/* Initialisation des autres attributs */
	xpos = 0, ypos = 0, zpos = 0, yrot = 0;
	TheGlWindowSize=Size(screen_width,screen_height);
	TheCaptureFlag=true; 
	facteurZoom = 0.125f; //valeur pour tuture.obj
    
}

/** Initialisation d'OpenGl (vue, etc.) */
void FonctionsOpenGL::initialisation (std::string obj)
{
     glClearColor(0.0, 0.0, 0.0, 0.0); // Met le fond en noir si aucune vid�o n'est charg� (cas limite)
	
    // Initialisation du viewport (f�netre d'affichage)
    glViewport(0,0,screen_width,screen_height);  

    // Passage en mode Projection 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // Initialisation de la matrice de projection � l'identit�
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,0.5f,100000000.0f);     


	//Initialisation et activation des lumi�res (utile)
   	GLfloat light_ambient[]= { 0.1f, 0.1f, 0.1f, 0.1f };
	GLfloat light_diffuse[]= { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat light_specular[]= { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat light_position[]= { 100.0f, 0.0f, -10.0f, 1.0f };
	glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_specular);
    glLightfv (GL_LIGHT1, GL_POSITION, light_position);    
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);

    //Initialisation et activitation des mat�riaux (utile ?)
	GLfloat mat_ambient[]= { 0.5f, 0.5f, 0.0f, 0.0f };
	GLfloat mat_diffuse[]= { 0.5f, 0.5f, 0.0f, 0.0f };
	GLfloat mat_specular[]= { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat mat_shininess[]= { 1.0f };
	glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_specular);
    glMaterialfv (GL_FRONT, GL_POSITION, mat_shininess);

	//Initialisations diverses d'OpenGl
    glShadeModel(GL_SMOOTH); // Type des Shaders utilis�s
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Correction de la perspective du texture mapping
    glEnable(GL_TEXTURE_2D); // Activation des textures
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Les polygones sont "remplies"
	glEnable(GL_CULL_FACE); // Activation du back face culling
   
	//Chargement .obj
	for (int i=0;i<1;i++)
	{
		objarray[i] = new (object_type);
		objarray[i]->objloader(obj);
		objarray[i]->objdatadisplay();      
	}

}


/** Fonction appel�e d�s qu'un redimensionnement de la taille de la fen�tre est effectu�.
 *
 * @param p_width : largeur (pixels) de la vue (fen�tre)
 * @param p_height : hauteur (pixels) de la vue (fen�tre)
 */
void FonctionsOpenGL::resize (int p_width, int p_height)
{
	if (screen_width==0 && screen_height==0) exit(0);

	screen_width=p_width; // On obtient la nouvelle taille de la fen�tre : ici largeur
	screen_height=p_height; // et l�, hauteur

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // On supprime les buffers de couleurs et de profondeurs pour la nouvelle image
	glViewport(0,0,screen_width,screen_height); // Transformation de la fen�tre

	glMatrixMode(GL_PROJECTION); // Transformation de la matrice
	glLoadIdentity(); // On r�initialise la matrice de projection � l'identit�
	gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,0.1f,1000.0f);

	glutPostRedisplay (); // On re-rend la sc�ne
	TheGlWindowSize=Size(screen_width,screen_height); //on sauvegarde la taille de la fen�tre pour la suite
}

/** Gestion du clavier.
 * Prise en charge de la taille de l'objet (+/- et r�init � 0)
 * et d�placements cam�ra/objet (pour d�bug principalement)
 */
void FonctionsOpenGL::keyboard (unsigned char key, int x, int y) {

	/* Gestion du zoom */
	if (key=='+')
		facteurZoom=facteurZoom*2;
	if (key=='-')
		facteurZoom=facteurZoom/2;
	if (key=='0')
		facteurZoom=1;

	/* Fl�ches directionnelles */
	if (key=='z') //avancer
	{
		zpos += TheMarkerSize;
	}
	if (key=='s') //reculer
	{
		zpos -= TheMarkerSize;
	}
    if (key=='q') //tourner � gauche (en avancant)
    {
		float distanceAZero = sqrt (xpos*xpos + zpos*zpos) / TheMarkerSize;
		yrot += 40/distanceAZero; 
		
		float yrotrad = (yrot / 180 * 3.141592654f);
		xpos += float(sin(yrotrad))*TheMarkerSize;
		zpos += float(cos(yrotrad))*TheMarkerSize;
    }
    if (key=='d') //tourner � droite (en avancant)
    {
		float distanceAZero = sqrt (xpos*xpos + zpos*zpos) / TheMarkerSize;
		yrot -= 20/distanceAZero; 

		float yrotrad = (yrot / 180 * 3.141592654f);
		xpos -= float(sin(yrotrad))*TheMarkerSize;
		zpos += float(cos(yrotrad))*TheMarkerSize;
    }
	if (key=='1')  // reinitialiser position voiture
		xpos = 0, ypos = 0, zpos = 0, yrot = 0;

	/* Touche d'�chappement (Echap = quitter) */
    if (key==27)
    {
		exit(0);
    }
}

/** Support de la souris.
 * Mouvement et clic. Pas utilis� pour le moment
*/
void FonctionsOpenGL::mouseMovement(int x, int y) {
	
}


/** G�n�re l'image en cours 
 * SUBROUTINE display(void)
 *
 * This is our main rendering subroutine, called each frame
 * 
 */
void FonctionsOpenGL::display(void)
{
   glDisable(GL_LIGHTING);
   
     
    if (TheResizedImage.rows==0) //On attend que l'image soit bien r�initialis�e avant de continuer
        return;
    ///C'est bon, image reinitialisee
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   
   // Activation Z-Buffer
   glDisable(GL_DEPTH_TEST);
   
    ///On rend l'image dans le buffer
    glMatrixMode(GL_MODELVIEW); //Positionnement de la camera
    glLoadIdentity();
    glPushMatrix();
   
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
	 glOrtho(0, TheGlWindowSize.width, 0, TheGlWindowSize.height, -1.0, 5.0);
    glViewport(0, 0, TheGlWindowSize.width , TheGlWindowSize.height);
    glDisable(GL_TEXTURE_2D);
    glPixelZoom( 1, -1);
    glRasterPos3f( 0, TheGlWindowSize.height  - 0.5, -1.0f );
    glDrawPixels ( TheGlWindowSize.width , TheGlWindowSize.height , GL_RGB , GL_UNSIGNED_BYTE , TheResizedImage.ptr(0) ); //rend la vid�o
	
   glPopMatrix();
   
   // Activation Z-Buffer
   glEnable(GL_DEPTH_TEST);
   
    ///On r�cup�re la matrice de projection afin de faire nos rendus dans l'environnement comme si on filmait depuis la cam�ra
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    double proj_matrix[16];

    TheCameraParams.glGetProjectionMatrix(TheInputImage.size(),TheGlWindowSize,proj_matrix,0.05,facteurZoom*10);
	
    glLoadIdentity();
    glLoadMatrixd(proj_matrix);
    glLineWidth(2);
    //Pour chaque marqueur (d�mo plus)
    double modelview_matrix[16];

	// Afficher un cube au dessus de chaque marker
   /*
	for (unsigned int m=0;m<TheMarkers.size();m++)
        {
            TheMarkers[m].glGetModelViewMatrix(modelview_matrix);
            glMatrixMode(GL_MODELVIEW);
           
            glLoadIdentity();
            glLoadMatrixd(modelview_matrix);
    // 		axis(TheMarkerSize);
            glColor3f(1,0.4,0.4);
            glTranslatef(0, TheMarkerSize/2,0);
           
            glutWireCube( TheMarkerSize );

        }
	*/

    //Si la planche est detectee avec assez de probabilites, on affiche l'objet
    if (TheBoardDetected.second>0.1) {
        //TheBoardDetected.first.glGetModelViewMatrix(modelview_matrix);
       TheMarkers[0].glGetModelViewMatrix(modelview_matrix);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glLoadMatrixd(modelview_matrix);
        glColor3f(0,1,0);
        glTranslatef(0, TheMarkerSize/2,0); //On est pile sur le plan des markers
       

        // Desactivation du color buffer pour dessiner le cube virtuel
        glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		
        displayVirtualHiddenWorld();
   
        // Reactivation du color buffer pour dessiner le cube virtuel
        glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
       
        displayVirtualWorld();
       
       glPopMatrix();
       
    }

   glPopMatrix(); // from the GL PROJECTION

    glutSwapBuffers();


}

/** vIdle, d�tecte les marqueurs entre chaque it�ration **/
void FonctionsOpenGL::vIdle()
{
    if (TheCaptureFlag) {
        //capture image
        TheVideoCapturer.grab();
        TheVideoCapturer.retrieve( TheInputImage);
        TheUndInputImage.create(TheInputImage.size(),CV_8UC3);
        //by deafult, opencv works in BGR, so we must convert to RGB because OpenGL in windows preffer
        cv::cvtColor(TheInputImage,TheInputImage,CV_BGR2RGB);
        //remove distorion in image
        cv::undistort(TheInputImage,TheUndInputImage, TheCameraParams.CameraMatrix,TheCameraParams.Distorsion);
        //detect markers
        MDetector.detect(TheUndInputImage,TheMarkers,TheCameraParams.CameraMatrix,Mat(),TheMarkerSize);
        //Detection of the board
        TheBoardDetected.second=TheBoardDetector.detect( TheMarkers, TheBoardConfig,TheBoardDetected.first, TheCameraParams,TheMarkerSize);
        //chekc the speed by calculating the mean speed of all iterations
        //resize the image to the size of the GL window
        cv::resize(TheUndInputImage,TheResizedImage,TheGlWindowSize);
    }
    glutPostRedisplay();
}



int FonctionsOpenGL::getWidth() {
	return screen_width;
}


int FonctionsOpenGL::getHeight() {
	return screen_height;
}

void FonctionsOpenGL::displayVirtualHiddenWorld()
{
   // Grossir/r�duire les �l�ments affich�s � l'�cran (+ pour zoomer, - pour d�zoomer, 1 pour revenir � la taille d'origine)
   glScalef(facteurZoom, facteurZoom, facteurZoom);
   
   // Dessin du cube
   glutSolidCube(TheMarkerSize*2);
}

void FonctionsOpenGL::displayVirtualWorld()
{
   glutWireCube(TheMarkerSize*2);
   
   // Affichage objet
    if (objarray[0]->id_texture!=-1)
    {
    glBindTexture(GL_TEXTURE_2D, objarray[0]->id_texture); // On active les textures
    glEnable(GL_TEXTURE_2D); // Texture mapping ok
    //printf("Textures charg�es");
    }
    else
    glDisable(GL_TEXTURE_2D); // Texture mapping OFF
   
   // Rotation de la voiture dans le plan (xOz)
   glRotatef(yrot,0.0,1.0,0.0);
   
   // Translation de la voiture dans le plan (xOz)
   glTranslated(xpos,0.0f,zpos);
   
   glScalef(facteurZoom, facteurZoom, facteurZoom);
   
   // Affichage de la voiture
   glColor3f(0,0,1);
   objarray[0]->render();
   

}

