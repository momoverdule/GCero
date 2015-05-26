#ifndef PLAYER_H_
#define PLAYER_H_
#include "navePj.h"

using namespace std;


class Player{
    
private:
	int gold;
	int score;
	//string name;




public:
    Player();
    ~Player();
	NavePj* nave;



};

#endif