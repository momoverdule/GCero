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
			rotacion = -0.03;
			orbita = 0;
			break;
		}
		case 1: { //planeta
			rotacion = ((rand()%3)+1);
			orbita = ((((rand()%8)+1)/10))+0.5;
			break;
		}
		case 2: { //luna
			rotacion = (((rand()%10)+1)/10)+2;
			orbita = (((rand()%20)+1)/10)+2;
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


