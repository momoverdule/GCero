#include "planeta.h"
#include <Ogre.h>
#include <iostream>

using namespace std;
using namespace Ogre;


Planeta::Planeta(SceneNode* _nodo, int _tipo, String name){

	nombre = name;
	nodo = _nodo;
	tipo = _tipo;

	switch (tipo){
		
		case 0: { //sol
			rotacion = -0.01;
			orbita = 0;
			break;
		}
		case 1: { //planeta
			rotacion = (rand()%5);
			orbita = (((rand()%10)+20)/10);
			break;
		}
		case 2: { //luna
			rotacion = (rand()%5);
			orbita = (rand()%5);
			break;
		}
			
	}
}

Planeta::~Planeta(){
	delete nodo;

}

//getters:
SceneNode* Planeta::getNode(){
	return nodo;
}


