#ifndef LINEA_H_
#define LINEA_H_
#include <iostream>


using namespace std;


class Linea{
    
private:


public:
    Linea(string _texto);
    Linea (const Linea &obj);
    ~Linea();

    string texto;

    Linea operator= (const Linea &linea2);

};

#endif