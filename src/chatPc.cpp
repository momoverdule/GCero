#include "chatPc.h"
#include <string>
#include <sstream>

using namespace std;

chatPc::chatPc(){

	Linea *vacio;
	//lineas.resize(5);

	for (int i = 0; i < 7; ++i)
	{
		vacio = new Linea(" ");
		lineas.push_back(*vacio);
	}

}

chatPc::~chatPc(){
	/*std::deque <Linea *>::iterator
 	itLinea = lineas.begin();
  	while (lineas.end() != itLinea); 
  	{
  		delete itLinea; ++itLinea;
 	}*/
}

void chatPc::addLinea(string _texto){

	Linea linea(_texto);
	if (lineas.size() >= 7) lineas.pop_front();
	lineas.push_back(linea);

}


string chatPc::screen(){
	
	std::stringstream sumatoria;

	std::deque <Linea >::iterator
 	itLinea = lineas.begin();

 	for (int i = 0; i < 7; ++i)
 	 {
 	 	sumatoria << /*"pepe ";*/ itLinea->texto;
  		++itLinea;
 	 } 
 	return sumatoria.str();
}





