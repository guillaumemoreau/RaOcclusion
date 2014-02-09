#include "cameraPos.h"
#include "objloader.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>


using namespace cv;
using namespace aruco;


#ifdef __WIN32__
#include <windows.h>
#endif

#include "GL\glut.h"  //glut has all ogl relevant .h files included 

//angle of rotation
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;

float cRadius = 10.0f; // our radius distance from our character

float lastx, lasty;

object_type *objarray[2];  //objects container for our world. Used throughout so global

//Lights settings
GLfloat light_ambient[]= { 0.1f, 0.1f, 0.1f, 0.1f };
GLfloat light_diffuse[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat light_specular[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat light_position[]= { 100.0f, 0.0f, -10.0f, 1.0f };

//Materials settings
GLfloat mat_ambient[]= { 0.5f, 0.5f, 0.0f, 0.0f };
GLfloat mat_diffuse[]= { 0.5f, 0.5f, 0.0f, 0.0f };
GLfloat mat_specular[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat mat_shininess[]= { 1.0f };

int screen_width=800,screen_height=600;

//Pour la détection des marqueurs => Camera Position par Aruco
string TheInputVideo,TheIntrinsicFile,TheBoardConfigFile;
float TheMarkerSize=-1;
VideoCapture TheVideoCapturer;
Mat TheInputImage,TheUndInputImage,TheResizedImage;
CameraParameters TheCameraParams;
Size TheGlWindowSize=Size(screen_width,screen_height);
bool TheCaptureFlag=true; 
MarkerDetector MDetector;
vector<Marker> TheMarkers;
//board
BoardDetector TheBoardDetector;
BoardConfiguration TheBoardConfig;
pair<Board,float> TheBoardDetected; //the board and its probability

//test clavier pour zoom
float facteurZoom = 1;
/************************************
 *
 * SUBROUTINE init(void)
 *
 * Used to initialize OpenGL and to setup our world
 *
************************************/

void init(string obj)
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // Clear background color to black
	
    // Viewport transformation
    glViewport(0,0,screen_width,screen_height);  

    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(70.0f,(GLfloat)screen_width/(GLfloat)screen_height,0.5f,100000000.0f);     


//Lights initialization and activation
    glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_specular);
    glLightfv (GL_LIGHT1, GL_POSITION, light_position);    
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);

    //Materials initialization and activation
	glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_specular);
    glMaterialfv (GL_FRONT, GL_POSITION, mat_shininess);    

//Other initializations
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Texture mapping perspective correction
    glEnable(GL_TEXTURE_2D); // Texture mapping ON
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
	glEnable(GL_CULL_FACE); // Enable the back face culling
    //glEnable(GL_DEPTH_TEST); // Cache les éléments normalement cachés : c'est le Z-Buffer

for (int i=0;i<2;i++)
{
printf("*************\n");
    objarray[i] = new (object_type);
    objarray[i]->objloader(obj);
    objarray[i]->objdatadisplay();      
}

}


/**********************************************************
 *
 * SUBROUTINE resize(int p_width, int p_height)
 *
 * This routine must be called everytime we resize our window.
 *
 * Input parameters: p_width = width in pixels of our viewport
 *                   p_height = height in pixels of our viewport
 * 
 *********************************************************/

void resize (int p_width, int p_height)
{
if (screen_width==0 && screen_height==0) exit(0);
    screen_width=p_width; // We obtain the new screen width values and store it
    screen_height=p_height; // Height value

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We clear both the color and the depth buffer so to draw the next frame
    glViewport(0,0,screen_width,screen_height); // Viewport transformation

    glMatrixMode(GL_PROJECTION); // Projection transformation
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,1.0f,100000000.0f);

    glutPostRedisplay (); // This command redraw the scene (it calls the same routine of glutDisplayFunc)
	TheGlWindowSize=Size(screen_width,screen_height); //aruco
}

/**********************************************************
 *
 * SUBROUTINE keyboard(void)
 *
 * Subroutine to handle keyboard input
 * 
 *********************************************************/


void keyboard (unsigned char key, int x, int y) {
    if (key=='q')
    {
    xrot += 1;
    if (xrot >360) xrot -= 360;
    }

	if (key=='+')
		facteurZoom=facteurZoom*2;
	if (key=='-')
		facteurZoom=facteurZoom/2;
	if (key=='0')
		facteurZoom=1;
    if (key=='z')
    {
    xrot -= 1;

if (xrot < -360) xrot += 360;
    }

    if (key=='w')
    {
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f); 
    xpos += float(sin(yrotrad));
    zpos -= float(cos(yrotrad));
    ypos -= float(sin(xrotrad));
    }

    if (key=='s')
    {
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f); 
    xpos -= float(sin(yrotrad));
    zpos += float(cos(yrotrad));
    ypos += float(sin(xrotrad));
    }

    if (key=='d')
    {
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos += float(cos(yrotrad)) * 0.2;
    zpos += float(sin(yrotrad)) * 0.2;
    }

    if (key=='a')
    {
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos -= float(cos(yrotrad)) * 0.2;
    zpos -= float(sin(yrotrad)) * 0.2;
    }

    if (key==27)
    {
    exit(0);
    }
}

/**********************************************************
 *
 * SUBROUTINE mouseMovement(void)
 *
 * Subroutine to handle mouse input
 * 
 *********************************************************/

void mouseMovement(int x, int y) {
int diffx=x-lastx; //check the difference between the current x and the last x position
int diffy=y-lasty; //check the difference between the current y and the last y position
lastx=x; //set lastx to the current x position
lasty=y; //set lasty to the current y position
xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
yrot += (float) diffx;    //set the xrot to yrot with the addition of the difference in the x position
}


/**********************************************************
 *
 * SUBROUTINE display(void)
 *
 * This is our main rendering subroutine, called each frame
 * 
 *********************************************************/

void display(void)
{
    if (TheResizedImage.rows==0) //prevent from going on until the image is initialized
        return;
    ///clear
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    ///draw image in the buffer
    glMatrixMode(GL_MODELVIEW); //Positionnement de la caméra
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	 glOrtho(0, TheGlWindowSize.width, 0, TheGlWindowSize.height, -1.0, 1.0);
    glViewport(0, 0, TheGlWindowSize.width , TheGlWindowSize.height);
    glDisable(GL_TEXTURE_2D);
    glPixelZoom( 1, -1);
    glRasterPos3f( 0, TheGlWindowSize.height  - 0.5, -1.0 );
    glDrawPixels ( TheGlWindowSize.width , TheGlWindowSize.height , GL_RGB , GL_UNSIGNED_BYTE , TheResizedImage.ptr(0) );

    ///On récupère la matrice de projection afin de faire nos rendus dans l'environnement comme si on filmait depuis la caméra
    glMatrixMode(GL_PROJECTION);
    double proj_matrix[16];

    TheCameraParams.glGetProjectionMatrix(TheInputImage.size(),TheGlWindowSize,proj_matrix,0.05,facteurZoom*10);
    glLoadIdentity();
    glLoadMatrixd(proj_matrix);
    glLineWidth(2);
    //now, for each marker,
    double modelview_matrix[16];

    /*    for (unsigned int m=0;m<TheMarkers.size();m++)
        {
            TheMarkers[m].glGetModelViewMatrix(modelview_matrix);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glLoadMatrixd(modelview_matrix);
    // 		axis(TheMarkerSize);
            glColor3f(1,0.4,0.4);
            glTranslatef(0, TheMarkerSize/2,0);
            glPushMatrix();
            glutWireCube( TheMarkerSize );

            glPopMatrix();
        }*/
    //If the board is detected with enough probability
    if (TheBoardDetected.second>0.3) {
        TheBoardDetected.first.glGetModelViewMatrix(modelview_matrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glLoadMatrixd(modelview_matrix);
        glColor3f(0,1,0);
        glTranslatef(0, TheMarkerSize/2,0); //On est pile sur le plan des markers
        glPushMatrix();
		
		if (objarray[0]->id_texture!=-1) 
		{
			glBindTexture(GL_TEXTURE_2D, objarray[0]->id_texture); // We set the active texture 
			glEnable(GL_TEXTURE_2D); // Texture mapping ON
			//printf("Txt map ON");
		}
		else
			glDisable(GL_TEXTURE_2D); // Texture mapping OFF
		
		/**
		Permet de faire grossir/minimiser les éléments affichés à l'écran (+ pour zoomer, - pour dézoomer, 1 pour revenir à la taille d'origine)
		**/
		glScalef(facteurZoom, facteurZoom, facteurZoom);

		objarray[0]->render(facteurZoom);

        glutWireTeapot( TheMarkerSize );

        glPopMatrix();
    }

    glutSwapBuffers();


}

/** vIdle, détecte les marqueurs entre chaque itération **/

void vIdle()
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

/**********************************************************
 *
 * The main routine
 * 
 *********************************************************/


int main(int argc, char **argv)
{
	/** Pour débugger plus vite !
	if (a/rgc!=6) {
        cerr<<"Nombre d'arguments invalide"<<endl;
        cerr<<"Ordre demandé : objet (triagulaire) .obj (in.avi|live) boardConfig.yml  intrinsicsLogitech.yml   size "<<endl;
        return false;
    }**/
	
	// Lignes suivantes remplacées par le "command arguments" suivant : "$(RELEASE_DIR)/roue de charrue.obj" "$(RELEASE_DIR)/video.avi" "$(RELEASE_DIR)/board_meters.yml" "$(RELEASE_DIR)/intrinsicsLogitech.yml" "0.039"

	/*argv[1] = "voiture.obj";
	argv[2] = "video.avi";
	argv[3] = "board_meters.yml";
	argv[4] = "intrinsics.yml";
	argv[5] = "0.039";*/
	

	// .obj à récupérer
	std::string obj = argv[1];

	/** Dépendance à Aruco et OpenCv **/

	//vidéo
	TheInputVideo=argv[2];

	//Fichier config du board
    TheBoardConfigFile=argv[3];

	//Caméra calibration info
    TheIntrinsicFile=argv[4];

	//taille des marqueurs
    TheMarkerSize=atof(argv[5]);

	//Lecture des infos du board
        TheBoardConfig.readFromFile(TheBoardConfigFile);

        //Open video input source
        if (TheInputVideo=="live")  //read from camera
            TheVideoCapturer.open(0);
        else TheVideoCapturer.open(TheInputVideo);
        if (!TheVideoCapturer.isOpened())
        {
            cerr<<"Could not open video"<<endl;
            return -1;

        }

        //read first image
        TheVideoCapturer>>TheInputImage;
        //read camera paramters if passed
        TheCameraParams.readFromXMLFile(TheIntrinsicFile);
        TheCameraParams.resize( TheInputImage.size());

	/** fin dépendance aruco/openCv **/

    // We use the GLUT utility to initialize the window, to handle the input and to interact with the windows system
    glutInit(&argc, argv);    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	screen_width = TheInputImage.size().width;
	screen_height = TheInputImage.size().height;
	glutInitWindowSize(TheInputImage.size().width,TheInputImage.size().height);    
	glutInitWindowPosition(0,0);
    glutCreateWindow("Echap pour quitter");    
    glutDisplayFunc(display);
    glutIdleFunc(vIdle);
    glutReshapeFunc (resize);

	// (mouse) movement and keyboard
	//glutPassiveMotionFunc(mouseMovement); 
	glutKeyboardFunc (keyboard); 

	//initialisation de facteurZoom 
	init(obj);
    glutMainLoop();

    return(0);    
}