#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

using namespace std;


// Prototypes des fonctions implémentées
void InitGL();
void Draw();
void Reshape(int w, int h);
//bool loadOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals);
bool loadOBJ(const char * path, float * vertice, float * normals, float * textures, float * colours);



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

	// Lire .obj
	/*
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; 
	bool res = loadOBJ("cube.obj", vertices, uvs, normals);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	*/
	
	glEnableClientState(GL_VERTEX_ARRAY);
	float *vertices, *normals, *textures, *colours;
	loadOBJ("C:\\Users\\max\\Dropbox\\RAOcclusion\\Obj\\voiture.obj", vertices, normals, textures, colours);
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

/**
Loader .obj n°1
*/
#include <fstream>
#include "floatvector.h"
bool loadOBJ(const char * path, float * vertice, float * normals, float * textures, float * colours)
{
vector<FloatVector> ver,nor,tex,col;
vector<unsigned int> iv,it,in;
ifstream fichier (path,ifstream::in);
if (fichier.is_open())
  {
	string ligne;
    while ( getline (fichier,ligne) )
    {
        if(ligne[0]=='v') //Coordonnées de points (vertex, texture et normale)
		{
			if(ligne[1]==' ') //Vertex
			{
				char x[255],y[255],z[255];
				sscanf(ligne.c_str(),"v %s %s %s",x,y,z);
				ver.push_back(FloatVector(strtod(x,NULL),strtod(y,NULL),strtod(z,NULL)));
			}
			else if(ligne[1]=='t') //Texture
			{
				char x[255],y[255];
				sscanf(ligne.c_str(),"vt %s %s",x,y);
				tex.push_back(FloatVector(strtod(x,NULL),strtod(y,NULL)));
			}
			else if(ligne[1]=='n') //Normale
			{
				char x[255],y[255],z[255];
				sscanf(ligne.c_str(),"vn %s %s %s",x,y,z);
				nor.push_back(FloatVector(strtod(x,NULL),strtod(y,NULL),strtod(z,NULL)));
			}
		}
		else if(ligne[0]=='f') //Indice faces
		{
			ligne=doubleSlash(ligne); //On remplace "//" par "/1/" dans toute la ligne
			ligne=remplacerSlash(ligne); //On remplace les '/' par des espaces, ex : pour "f 1/2/3 4/5/6 7/8/9" on obtiendra "f 1 2 3 4 5 6 7 8 9"

			vector<string> termes=splitSpace(ligne.substr(2)); //On éclate la chaîne en ses espaces (le substr permet d'enlever "f ")

			int ndonnees=(int)termes.size()/3;
			for(int i=0;i<(ndonnees==3?3:4);i++) //On aurait très bien pu mettre i<ndonnees mais je veux vraiment limiter à 3 ou 4
			{
				iv.push_back(strtol(termes[i*3].c_str(),NULL,10)-1);
				it.push_back(strtol(termes[i*3+1].c_str(),NULL,10)-1);
				in.push_back(strtol(termes[i*3+2].c_str(),NULL,10)-1);
			}
			if(ndonnees==3) //S'il n'y a que 3 sommets on duplique le dernier pour faire un quad ayant l'apparence d'un triangle
			{
				iv.push_back(strtol(termes[0].c_str(),NULL,10)-1);
				it.push_back(strtol(termes[1].c_str(),NULL,10)-1);
				in.push_back(strtol(termes[2].c_str(),NULL,10)-1);
			}
		}
		//fin lecture ligne
		
    }
    fichier.close();
  }
  
    vector<float> tv(0),tc(0),tn(0),tt(0);
    for(unsigned int i=0;i<iv.size();i++)
        if(iv[i]<ver.size())
        {
            tv.push_back(ver[iv[i]].x);
            tv.push_back(ver[iv[i]].y);
            tv.push_back(ver[iv[i]].z);

            tc.push_back(col[i].x);
            tc.push_back(col[i].y);
            tc.push_back(col[i].z);
            tc.push_back(col[i].a);
        }

    for(unsigned int i=0;i<in.size();i++)
        if(in[i]<nor.size())
        {
            tn.push_back(nor[in[i]].x);
            tn.push_back(nor[in[i]].y);
            tn.push_back(nor[in[i]].z);
        }

    for(unsigned int i=0;i<it.size();i++)
        if(it[i]<tex.size())
        {
            tt.push_back(tex[it[i]].x);
            tt.push_back(tex[it[i]].y);
        }

    vertice=vector2float(tv);
    normals=vector2float(tn);
    textures=vector2float(tt);
    colours=vector2float(tc);
    

}

/** 
Loader .obj n°2
*/
bool loadOBJ2(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals)
{
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	
	}

	return true;
}
