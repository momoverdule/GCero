#ifndef CHATPC_H_
#define CHATPC_H_
#include "linea.h"
#include <deque>

using namespace std;


class chatPc{
    
private:
	std::deque  <Linea > lineas;


public:
    chatPc();
    ~chatPc();

    void addLinea(string _texto);
    string screen();



};

#endif