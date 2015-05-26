#include "player.h"
#include <iostream>
#include <Ogre.h>

using namespace std;
using namespace Ogre;

Player::Player(){
	nave = new NavePj(1000,500,Vector3(0,32800,0));

}