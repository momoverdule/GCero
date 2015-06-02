#include "linea.h"

using namespace std;

Linea::Linea(string _texto){
	texto =  _texto + "\n";
}

Linea::Linea (const Linea &obj) {
    cout << "Constructor copia..." << endl;
  texto = obj.texto;
}



Linea::~Linea(){

}

//sobrecarga de operadores
Linea Linea::operator= (const Linea &linea2){
  texto = linea2.texto;
}
