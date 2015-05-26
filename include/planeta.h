#ifndef PLANETA_H_
#define PLANETA_H_
#include <Ogre.h>

using namespace std;
using namespace Ogre;

class Planeta{
    
private:
	SceneNode* nodo;
	string nombre;




public:
    Planeta(SceneNode* _nodo, int _tipo, String name = "Planeta");
    ~Planeta();
    int tipo ;
    float rotacion;
    float orbita;

    //getters
    SceneNode* getNode();
    string getName();


};

#endif