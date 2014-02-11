#ifndef OBJLOAD
#define OBJLOAD

#include <stdio.h>
#include <iostream>
#include <io.h>
#include <stdlib.h>
#include "GL\glut.h" 

#define MAX_VERTICES 8000  // Nombre max de sommets 
#define MAX_POLYGONS 8000  //Nombre max de polygones
#define MAX_NORMALS 8000  // Nombre max de normales

/** Type Sommet */
typedef struct{
float x,y,z;
}vertex_type;

/** Type Normal */
typedef struct{
float i,j,k;
}normcoord_type;

/** Type Polygone (triangle). 3 entiers qui désigne 3 sommets */
typedef struct{
int v[3],t[3],n[3];
}polygon_type;

/** Type "mapcoord". 2 "texture coordinates" pour chaque sommet */
typedef struct{
float u,v;
}mapcoord_type;

/** Type "object" pour manipuler un fichier d'extension "obj".
* Permet de charger et afficher un fichier "obj". 
* Seuls les sommets, faces, normales et "texture maps" sont récupérés.
* Le chargement s'effectue vers les types (=struct) vertex_type, normcoord_type, polygon_type, mapcoord_type.
* L'affichage OpenGL peut être effectué directement grâce à la méthode "render()" (sans se préoccuper des types précédents).
*/
class object_type{
	public:
		object_type(){} /**< constructeur */
		~object_type(){} /**< destructeur */
		int objloader(std::string p_filename); /**< charger l'obj */
		int objdatadisplay(); /**< affichage textuelle de l'obj chargé */
		void render(); /**< affichage graphique (OpenGL) de l'obj chargé */
		int id_texture; /**< id de la texture à utiliser */
	private:
		int vertices_qty; 
		int polygons_qty; 
		int mapcoord_qty;
		int normcoord_qty;
		vertex_type vertex[MAX_VERTICES]; 
		mapcoord_type mapcoord[MAX_VERTICES];
		normcoord_type normcoord[MAX_NORMALS];
		polygon_type polygon[MAX_POLYGONS]; 
};
#endif