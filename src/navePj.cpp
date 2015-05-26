#include "navePj.h"
#include <iostream>
#include "yisus.h"

using namespace std;
using namespace Ogre;

int NavePj::numNaves = 0;

NavePj::NavePj() {

}


NavePj::NavePj(int hpNave, int enerNave, Vector3 pos){

    //inicializando cada parte de la nave
    casco.hpMax = (hpNave*40)/100;      //casco
    casco.hp = casco.hpMax;
    casco.func = true;
    generador.hpMax = (hpNave*10)/100;  //generador
    generador.hp = generador.hpMax;
    generador.func = true;
    generador.rate = 1;
    ala_I.hpMax = (hpNave*10)/100;      //ala_I
    ala_I.hp = ala_I.hpMax;
    ala_I.func = true;
    ala_D.hpMax = (hpNave*10)/100;      //ala_D
    ala_D.hp =  ala_D.hpMax;
    ala_D.func = true;
    prop_P.hpMax = (hpNave*5)/100;      //prop_P
    prop_P.hp = prop_P.hpMax;
    prop_P.func = true;
    prop_S.hpMax = (hpNave*5)/100;      //prop_S
    prop_S.hp = prop_S.hpMax;
    prop_S.func = true;
    computadora.hpMax = (hpNave*6)/100; //computadora
    computadora.hp = computadora.hpMax;
    computadora.func = true;
    sat1.hpMax = (hpNave*2)/100;        //sat1
    sat1.hp = sat1.hpMax;
    sat1.func = true;
    sat2.hpMax = (hpNave*2)/100;        //sat2
    sat2.hp = sat2.hpMax;
    sat2.func = true;
    arma_S.hpMax = (hpNave*5)/100;      //arma_S
    arma_S.hp = arma_S.hpMax;
    arma_S.func = true;
    defensas.hpMax = (hpNave*5)/100;    //defensas
    defensas.hp = defensas.hpMax;
    defensas.func = true;
    nanoBots.focus = 0;                 //nanoBots
    nanoBots.limitMax = 30;
    nanoBots.rate = 1;
    hpMax = hpNave;                     //satatsNave
    hp = hpMax;
    enerMax = enerNave;
    ener = enerMax/2;
    propulsion = 0;
    takingFire = false;
    
    numNaves++;

    //creando nodos de posicion:
    SceneManager* sMgr = Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");
    naveNodo = sMgr->createSceneNode("nave" + StringConverter::toString(numNaves));
    naveEnt = sMgr->createEntity("xWingEnt" + StringConverter::toString(numNaves),"xWing.mesh");
    naveNodo->attachObject(naveEnt);
    sMgr->getRootSceneNode()->addChild(naveNodo);
    naveNodo->setPosition(pos);
    naveNodo->setDirection(1,0,0);    
    
    //creando camaras:
    pCam = sMgr->createCamera("Principal");
    pCam->setPosition(Vector3(4.4,1.15,0));
    pCam->setDirection(1,0,0);
    pCam->setNearClipDistance(0.1);
    pCam->setFarClipDistance(100);
    pCam->setFOVy(Ogre::Degree(52));
    naveNodo->attachObject(pCam);


    //seteando posicion de armas:
    gun1 = naveNodo->createChildSceneNode("gun1");
    gun1->setDirection(Vector3(1,0,0));
    gun1->translate(Vector3(17,-1.7,8.3));
    

    gun2 = sMgr->createSceneNode("gun2");
    naveNodo->addChild(gun2);
    gun2->setDirection(1,0,0);
    gun2->translate(Vector3(17,-1.7,-8.3));
    


    //creando satelites:
    Entity* satEnt;
    sat1Nodo = sMgr->createSceneNode("satelite 1");
    sat1Cam = sMgr->createCamera("sat1Cam");
    sat1Nodo->attachObject(sat1Cam);
    naveNodo->addChild(sat1Nodo);
    satEnt = sMgr->createEntity("sat1","cassini.mesh");
    //gun1->attachObject(satEnt);
    sat1Cam->setDirection(0,0,-1);
    sat1Cam->setNearClipDistance(2);
    sat1Cam->setFarClipDistance(100);
    sat1Cam->setFOVy(Ogre::Degree(52));
    sat1Nodo->setPosition(Vector3(8,3.8,0));
    

    sat2Nodo = sMgr->createSceneNode("satelite 2");
    sat2Cam = sMgr->createCamera("sat2Cam");
    sat2Nodo->attachObject(sat2Cam);
    naveNodo->addChild(sat2Nodo);
    satEnt = sMgr->createEntity("sat2","cassini.mesh");
    //gun2->attachObject(satEnt);
    sat2Cam->setDirection(-1,0,0);
    sat2Cam->setNearClipDistance(2);
    sat2Cam->setFarClipDistance(100);
    sat2Cam->setFOVy(Ogre::Degree(52));
    sat2Nodo->setPosition( Vector3(7,8,4));



   //luces
  luzDelantera = sMgr->createLight("LuzDel");
  SceneNode* nodeLD = sMgr->createSceneNode();
  nodeLD->attachObject(luzDelantera);
  nodeLD->setPosition(Vector3(4,0,0));
  naveNodo->addChild(nodeLD);

  luzDelantera->setType(Ogre::Light::LT_SPOTLIGHT);
  luzDelantera->setDiffuseColour(1,1,1);
  luzDelantera->setDirection(Ogre::Vector3(1,-0.2,0));
  luzDelantera->setSpotlightInnerAngle(Ogre::Degree(0.00000001f));
  luzDelantera->setSpotlightOuterAngle(Ogre::Degree(40.0f));
  luzDelantera->setSpotlightFalloff(0.0000005f);
  luzDelantera->setAttenuation( 80, 1.0f, 4.5/10000, 75.0f/(2000*2000));
  //luzDelantera->setAttenuation( 1000, 1.0f, 4.5/1000, 75.0f/(1000*1000));

/*
   
  luzProp1 = sMgr->createLight("LuzProp1");
  luzProp2 = sMgr->createLight("LuzProp2");
  luzProp1->setDirection(Ogre::Vector3(1,0,0));
  luzProp2->setDirection(Ogre::Vector3(1,0,0));
  SceneNode* nodeL1 = sMgr->createSceneNode();
  SceneNode* nodeL2 = sMgr->createSceneNode();
  nodeL1->attachObject(luzProp1);
  nodeL2->attachObject(luzProp2);
  nodeL1->setPosition(pos + Vector3(-8,0,4));
  nodeL2->setPosition(pos + Vector3(-8,0,-4));
  naveNodo->addChild(nodeL1);
  naveNodo->addChild(nodeL2);

  luzProp1->setType(Ogre::Light::LT_SPOTLIGHT);
  luzProp1->setDiffuseColour(1,0,0);
  luzProp1->setDirection(Ogre::Vector3(1,0,0));
  luzProp1->setSpotlightInnerAngle(Ogre::Degree(0.5f));
  luzProp1->setSpotlightOuterAngle(Ogre::Degree(2.0f));
  luzProp1->setSpotlightFalloff(0.0000005f);

  luzProp2->setType(Ogre::Light::LT_SPOTLIGHT);
  luzProp2->setDiffuseColour(1,0,0);
  luzProp2->setDirection(Ogre::Vector3(1,0,0));
  luzProp2->setSpotlightInnerAngle(Ogre::Degree(0.5f));
  luzProp2->setSpotlightOuterAngle(Ogre::Degree(2.0f));
  luzProp2->setSpotlightFalloff(0.0000005f);

  */  

  

}


NavePj::~NavePj(){

}


void NavePj::lazerDer(){
    if (ala_D.func)
    {
        if (ener >= 8)
        {
            ener -= 8;
            Yisus::summonYisus()->disparar(pCam->getDerivedOrientation(), gun1->_getDerivedPosition(), pCam->getDerivedDirection().normalisedCopy(), propulsion*2);
        } else cout << "\n no energy" << endl; 
        
    } else {
         cout << "\n Ala derecha averiada" << endl; 
    }
}

void NavePj::lazerIzq(){
    if (ala_I.func) 
    {
       if (ener >= 8)
        {
            ener -= 8;
            Yisus::summonYisus()->disparar(pCam->getDerivedOrientation(), gun2->_getDerivedPosition(), pCam->getDerivedDirection().normalisedCopy(), propulsion*2);
        } else cout << "\n no energy" << endl; 
    }else{
        cout << "\n Ala derecha averiada" << endl; 
    }
}

void NavePj::aplicarDmg(int dmg){
    
    switch ((rand()%11)+1){

        case 1:{

            casco.hp -= dmg;

            if (casco.hp <= (casco.hpMax * 20)/100){
                casco.func = false;
                if (casco.hp < 0)
                 {
                     casco.hp = 0;
                 } 
            }  

            break;
        }
        case 2:{

            generador.hp -= dmg;

            if (generador.hp <= (generador.hpMax * 20)/100){
                generador.func = false;
                if (generador.hp < 0)
                 {
                     generador.hp = 0;
                 } 
            }  
            break;
        }
        case 3:{

            ala_I.hp -= dmg;

            if (ala_I.hp <= (ala_I.hpMax * 20)/100){
                ala_I.func = false;
                if (ala_I.hp < 0)
                 {
                     ala_I.hp = 0;
                 } 
            }

            break;
        }
        case 4:{
            ala_D.hp -= dmg;
            if (ala_D.hp <= (ala_D.hpMax * 20)/100){
                ala_D.func = false;
                if (ala_D.hp < 0)  ala_D.hp = 0;         
            }
            break;
        }
        case 5:{
            prop_S.hp -= dmg;
            if (prop_S.hp <= (prop_S.hpMax * 20)/100){
                prop_S.func = false;
                if (prop_S.hp < 0)  prop_S.hp = 0;         
            }
            break;
        }
        case 6:{
            computadora.hp -= dmg;
            if (computadora.hp <= (computadora.hpMax * 20)/100){
                computadora.func = false;
                if (computadora.hp < 0)  computadora.hp = 0;         
            }
            break;
        }
        case 7:{

            prop_P.hp -= dmg;
            if (prop_P.hp <= (prop_P.hpMax * 20)/100){
                prop_P.func = false;
                if (prop_P.hp < 0)  prop_P.hp = 0;         
            }
            break;
        }
        case 8:{
            sat1.hp -= dmg;
            if (sat1.hp <= (sat1.hpMax * 20)/100){
                sat1.func = false;
                if (sat1.hp < 0)  sat1.hp = 0;         
            }
            break;
        }
        case 9:{
            sat1.hp -= dmg;
            if (sat2.hp <= (sat2.hpMax * 20)/100){
                sat2.func = false;
                if (sat2.hp < 0)  sat2.hp = 0;         
            }

            break;
        }
        case 10:{
            arma_S.hp -= dmg;
            if (arma_S.hp <= (arma_S.hpMax * 20)/100){
                arma_S.func = false;
                if (arma_S.hp < 0)  arma_S.hp = 0;         
            }

            break;
        }
        case 11:{
            defensas.hp -= dmg;
            if (defensas.hp <= (defensas.hpMax * 20)/100){
                defensas.func = false;
                if (defensas.hp < 0)  defensas.hp = 0;         
            }

            break;
        }


    }


}

        /* focus switch list:
            0: inactive
            1: casco
            2: generador
            3: ala_i
            4: ala_d
            5: prop_s
            6: prop_p
            7: computadora
            8: sat1
            9: sat2
            10: arma_s
            11: defensa
        */



void NavePj::generateEner(Ogre::Real deltaT) {
    ener += 65 * generador.rate * deltaT;
    if (ener > enerMax) ener = enerMax;
    if (ener < 0) ener = 0;
}

void NavePj::regenerateHp(Ogre::Real deltaT){
    switch (nanoBots.focus){
        
        case 1: {   //casco
            casco.hp += 15 * nanoBots.rate * deltaT;
            if (casco.hp >= (casco.hpMax * nanoBots.limitMax)/100 ){    //limite de reparacion maximo
                nanoBots.focus = 0;
            }
            if (casco.hp >= (casco.hpMax * 20)/100){
                casco.func = true;                                      //reactivando el funcionamiento
            } 
            break;
        }

        case 2: {   //generador
            generador.hp += 15 * nanoBots.rate * deltaT;
            if (generador.hp >= (generador.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if (generador.hp >= (generador.hpMax * 20)/100){
                generador.func = true; 
            }  
            break;
        }
        case 3: {   //ala_I
            ala_I.hp += 15 * nanoBots.rate * deltaT;
            if (ala_I.hp >= (ala_I.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if (ala_I.hp >= (ala_I.hpMax * 20)/100){
                ala_I.func = true; 
            }  
            break;
        }
        case 4: {   //ala_D
            ala_D.hp += 15 * nanoBots.rate * deltaT;
            if (ala_D.hp >= (ala_D.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if (ala_D.hp >= (ala_D.hpMax * 20)/100){
                ala_D.func = true; 
            }  
            break;
        }
        case 5: {   //propulsor_S
            prop_S.hp += 15 * nanoBots.rate * deltaT;
            if (prop_S.hp >= (prop_S.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if (prop_S.hp >= (prop_S.hpMax * 20)/100){
                prop_S.func = true; 
            }  
            break;
        }
        case 6: {   //propulsor_P
            prop_P.hp += 15 * nanoBots.rate * deltaT;
            if (prop_P.hp >= (prop_P.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if (prop_P.hp >= (prop_P.hpMax * 20)/100){
                prop_P.func = true; 
            }  
            break;
        }
        case 7: {   //computadora
            computadora.hp += 15 * nanoBots.rate * deltaT;
            if (computadora.hp >= (computadora.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if (computadora.hp >= (computadora.hpMax * 20)/100){
                computadora.func = true; 
            }  
            break;
        }
        case 8: {   //sat1
            sat1.hp += 15 * nanoBots.rate * deltaT;
            if (sat1.hp >= (sat1.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if (sat1.hp >= (sat1.hpMax * 20)/100){
                sat1.func = true; 
            }  
            break;
        }
        case 9: {   //sat2
            sat2.hp += 15 * nanoBots.rate * deltaT;
            if (sat2.hp >= (sat2.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if (sat2.hp >= (sat2.hpMax * 20)/100){
                sat2.func = true; 
            }  
            break;
        }
        case 10: {   //arma_S
            arma_S.hp += 15 * nanoBots.rate * deltaT;
            if (arma_S.hp >= (arma_S.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if (arma_S.hp >= (arma_S.hpMax * 20)/100){
                arma_S.func = true; 
            }  
            break;
        }
        case 11: {   //defensas
            defensas.hp += 15 * nanoBots.rate * deltaT;
            if (defensas.hp >= (defensas.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if (defensas.hp >= (defensas.hpMax * 20)/100){
                defensas.func = true; 
            }  
            break;
        }
    }
}






//calculos por frame:
void NavePj::calculate(Ogre::Real deltaT){
    if (nanoBots.focus != 0) regenerateHp(deltaT);    //reparacion
    if (generador.func) generateEner(deltaT);    //carga de energia
    if (nanoBots.focus == 0) nanoBots.focus = setNanoBotsTarget();  //busqueda de reparaciones
    
    
    //propulsores
    naveNodo->translate(Vector3(pCam->getDerivedDirection() * propulsion*2));   //aplicar propulsion
    if ((propulsion/15) < ener){
        ener -= propulsion/15;
    } else {
        propulsion -= 0.2;
        if (propulsion < 0) propulsion = 0;
        cout << "\n sin energia" << endl;
    }

    hp = hpActual();
    sat2Nodo->pitch(Radian(0.005),Node::TS_PARENT);
    sat2Nodo->yaw(Radian(0.002),Node::TS_PARENT);
}




int NavePj::setNanoBotsTarget(){

    int target;


    //prioridades
    if (casco.hp < (casco.hpMax * nanoBots.limitMax)/100)
    {
        target = 1;
    } else if (generador.hp < (generador.hpMax * nanoBots.limitMax)/100)
        {
            target = 2;
        } else if (ala_I.hp < (ala_I.hpMax * nanoBots.limitMax)/100)
            {
                target = 3;
            } else if (ala_D.hp < (ala_D.hpMax * nanoBots.limitMax)/100)
                {
                    target = 4;
                } else if (prop_P.hp < (prop_P.hpMax * nanoBots.limitMax)/100)
                    {
                        target = 6;
                    } else if (prop_S.hp < (prop_S.hpMax * nanoBots.limitMax)/100)
                        {
                            target = 5;
                        } else if (computadora.hp < (computadora.hpMax * nanoBots.limitMax)/100)
                            {
                                target = 7;
                            } else if (defensas.hp < (defensas.hpMax * nanoBots.limitMax)/100)
                                {         
                                    target = 11;    
                                } else if (sat1.hp < (sat1.hpMax * nanoBots.limitMax)/100)
                                    {
                                        target = 8;
                                    } else if (sat2.hp < (sat2.hpMax * nanoBots.limitMax)/100)
                                        {
                                            target = 9;
                                        } else if (arma_S.hp < (arma_S.hpMax * nanoBots.limitMax)/100)
                                            {
                                                target = 10;
                                            } 

    cout << "\nbots target: " << target << endl;

            /* focus switch list:
            0: inactive
            1: casco
            2: generador
            3: ala_i
            4: ala_d
            5: prop_s
            6: prop_p
            7: computadora
            8: sat1
            9: sat2
            10: arma_s
            11: defensa
        */   
    return target;
}

int NavePj::hpActual(){
    int hpActual = 0;

    hpActual += casco.hp;
    hpActual += generador.hp;
    hpActual += ala_I.hp;
    hpActual += ala_D.hp;
    hpActual += prop_S.hp;
    hpActual += prop_P.hp;
    hpActual += computadora.hp;
    hpActual += sat1.hp;
    hpActual += sat2.hp;
    hpActual += arma_S.hp;
    hpActual += defensas.hp;

    return hpActual;
}

            /* focus switch list:
            0: inactive
            1: casco
            2: generador
            3: ala_i
            4: ala_d
            5: prop_s
            6: prop_p
            7: computadora
            8: sat1
            9: sat2
            10: arma_s
            11: defensa
        */   





//setters:
void NavePj::setCamP(){
   viewP->setCamera(pCam);
}
void NavePj::setCamS1(){
   viewP->setCamera(sat1Cam);
}
void NavePj::setCamS2(){
   viewP->setCamera(sat2Cam);
}
void NavePj::setViewPort(Viewport* port){
    viewP = port;
}
void NavePj::setNanoBotsTarget(int target){
    nanoBots.focus = target;
}


void NavePj::setRigidBody(OgreBulletDynamics::RigidBody* body){
    rigidBody = body;
}

void NavePj::setPropulsion(int signo){


    if (signo > 0){
        if (propulsion/15 < ener) propulsion += 0.5;
    }else propulsion -= 0.2;
    if (propulsion > 20) propulsion = 20;
    if (propulsion < 0) propulsion = 0;
    cout << "\n propulsion " <<  propulsion << " = " << (propulsion*100)/20 << endl;

}

void NavePj::setNode(SceneNode* nodo){
    naveNodo = nodo;
}


//-------------------getters:
Vector3 NavePj::getPosition(){
    return naveNodo->getPosition();
}

Quaternion NavePj::getDirectionQuad(){
    return pCam->getDerivedOrientation();
}

Entity* NavePj::getEntity(){
    return naveEnt;
}

SceneNode* NavePj::getNode(){
    return naveNodo;
}

OgreBulletDynamics::RigidBody* NavePj::getRigidBody(){
    return rigidBody;
}

Ogre::Camera* NavePj::getSat1Cam(){
    return sat1Cam;
}

Ogre::Camera* NavePj::getPCam(){
    return pCam;
}

int NavePj::getProp(){
    return (propulsion*100)/20;
}
int NavePj::getPropNeta(){
    return propulsion;
}
int NavePj::getEner(){
    return (ener*100)/enerMax;
}
Vector3 NavePj::getDirection(){
    return  pCam->getDerivedDirection().normalisedCopy();
}
Vector3 NavePj::getGun1(){
    return gun1->_getDerivedPosition();
}
Vector3 NavePj::getGun2(){
    return gun2->_getDerivedPosition();
}

int NavePj::getHp(){
    return (hp*100)/hpMax;;
}

//----------------mandos del avion:
void NavePj::yaw(int signo){                 //yaw
   naveNodo->yaw(Radian(signo*0.01));
}

void NavePj::pitch(int signo){               //pitch
   naveNodo->pitch(Radian(signo*0.001));
}

void NavePj::roll(int signo){                //roll
   naveNodo->roll(Radian(signo*0.001));
}










