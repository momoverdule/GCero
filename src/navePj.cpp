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
    consola = new chatPc();
    anguloGiro = new btVector3(0,0,0);
    
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
    gun1->translate(Vector3(20,-1.7,8.3));
    

    gun2 = sMgr->createSceneNode("gun2");
    naveNodo->addChild(gun2);
    gun2->setDirection(1,0,0);
    gun2->translate(Vector3(20,-1.7,-8.3));
    


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


  _overlayManager = OverlayManager::getSingletonPtr();

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

    delete naveNodo;
    delete gun2;
    delete gun1;
    delete sat1Nodo;
    delete sat2Nodo;
    delete rigidBody;
    delete naveEnt;
    delete pCam;
    delete sat1Cam;
    delete sat2Cam;
    delete viewP;
    delete luzDelantera;
    delete luzProp1;
    delete luzProp2;
    delete _overlayManager;
    delete consola;

}


void NavePj::lazerDer(){
    if (ala_D.func)
    {
        if (ener >= 8)
        {
            ener -= 8;
            Yisus::summonYisus()->disparar(pCam->getDerivedOrientation(), gun1->_getDerivedPosition(), pCam->getDerivedDirection().normalisedCopy(), propulsion*2);
        } else consola->addLinea("No energy"); 
        
    } else {
         consola->addLinea("Cañon Izquierdo averiado: **Imposible disparar**"); 
    }
}

void NavePj::lazerIzq(){
    if (ala_I.func) 
    {
       if (ener >= 8)
        {
            ener -= 8;
            Yisus::summonYisus()->disparar(pCam->getDerivedOrientation(), gun2->_getDerivedPosition(), pCam->getDerivedDirection().normalisedCopy(), propulsion*2);
        } else consola->addLinea("Energia insuficiente"); 
    }else{
        consola->addLinea("Cañon Derecho averiado: **Imposible disparar**");
    }
}

void NavePj::aplicarDmg(int dmg){
    
    Ogre::OverlayElement *oe;
    bool flag = false;

    while ((!flag) && (hp > 0)){
    switch ((rand()%11)+1){

        case 1:{
            if(casco.hp > 0) flag = true;
            casco.hp -= dmg;
            oe =  _overlayManager->getOverlayElement("casco");
            if ((casco.hp <= (casco.hpMax * 50)/100) && (casco.hp > (casco.hpMax * 20)/100)) oe->setMaterialName("cascoN");

            if (casco.hp <= (casco.hpMax * 20)/100){
                casco.func = false;
                if (casco.hp < 0)
                 {
                     casco.hp = 0;
                 }
                 
                 oe->setMaterialName("cascoR");
            }  

            break;
        }
        case 2:{
            if(generador.hp > 0) flag = true;
            generador.hp -= dmg;
            oe =  _overlayManager->getOverlayElement("gen");
            if ((generador.hp <= (generador.hpMax * 50)/100) && (generador.hp > (generador.hpMax * 20)/100)) oe->setMaterialName("genN");

            if (generador.hp <= (generador.hpMax * 20)/100){
                generador.func = false;
                if (generador.hp < 0)
                 {
                     generador.hp = 0;
                 } 
                 oe->setMaterialName("genR");
            }  
            break;
        }
        case 3:{
            if(ala_I.hp > 0) flag = true;
            ala_I.hp -= dmg;
            oe = _overlayManager->getOverlayElement("alaIzq");
            if ((ala_I.hp <= (ala_I.hpMax * 50)/100) && (ala_I.hp > (ala_I.hpMax * 20)/100)) oe->setMaterialName("alaIzqN");

            if (ala_I.hp <= (ala_I.hpMax * 20)/100){
                ala_I.func = false;
                if (ala_I.hp < 0)
                 {
                     ala_I.hp = 0;
                 } 
                 oe->setMaterialName("alaIzqR");
            }

            break;
        }
        case 4:{
            if(ala_D.hp > 0) flag = true;
            ala_D.hp -= dmg;
            oe = _overlayManager->getOverlayElement("alaDer");
            if ((ala_D.hp <= (ala_D.hpMax * 50)/100) && (ala_D.hp > (ala_D.hpMax * 20)/100)) oe->setMaterialName("alaDerN");
            if (ala_D.hp <= (ala_D.hpMax * 20)/100){
                ala_D.func = false;
                if (ala_D.hp < 0)  ala_D.hp = 0;   
                oe->setMaterialName("alaDerR");      
            }
            break;
        }
        case 5:{
            if(prop_S.hp > 0) flag = true;
            prop_S.hp -= dmg;
            oe = _overlayManager->getOverlayElement("propS");
            if ((prop_S.hp <= (prop_S.hpMax * 50)/100) && (prop_S.hp > (prop_S.hpMax * 20)/100)) oe->setMaterialName("propSN");
            if (prop_S.hp <= (prop_S.hpMax * 20)/100){
                prop_S.func = false;
                if (prop_S.hp < 0)  prop_S.hp = 0;  
                oe->setMaterialName("propSR"); 

            }
            break;
        }
        case 6:{
            if(computadora.hp > 0) flag = true;
            computadora.hp -= dmg;
            oe = _overlayManager->getOverlayElement("comp");
            if ((computadora.hp <= (computadora.hpMax * 50)/100) && (computadora.hp > (computadora.hpMax * 20)/100)) oe->setMaterialName("compN");
            if (computadora.hp <= (computadora.hpMax * 20)/100){
                computadora.func = false;
                if (computadora.hp < 0)  computadora.hp = 0; 
                oe->setMaterialName("compR");        
            }
            break;
        }
        case 7:{
            if(prop_P.hp > 0) flag = true;
            prop_P.hp -= dmg;
            oe = _overlayManager->getOverlayElement("propP");
            if ((prop_P.hp <= (prop_P.hpMax * 50)/100) && (prop_P.hp > (prop_P.hpMax * 20)/100)) oe->setMaterialName("propPN");
            if (prop_P.hp <= (prop_P.hpMax * 20)/100){
                prop_P.func = false;
                if (prop_P.hp < 0)  prop_P.hp = 0; 
                oe->setMaterialName("propPR");         
            }
            break;
        }
        case 8:{
            if(sat1.hp > 0) flag = true;
            sat1.hp -= dmg;
            if (sat1.hp <= (sat1.hpMax * 20)/100){
                sat1.func = false;
                if (sat1.hp < 0)  sat1.hp = 0;         
            }
            break;
        }
        case 9:{
            if(sat2.hp > 0) flag = true;
            sat2.hp -= dmg;
            if (sat2.hp <= (sat2.hpMax * 20)/100){
                sat2.func = false;
                if (sat2.hp < 0)  sat2.hp = 0;         
            }

            break;
        }
        case 10:{
            if(arma_S.hp > 0) flag = true;
            arma_S.hp -= dmg;
            if (arma_S.hp <= (arma_S.hpMax * 20)/100){
                arma_S.func = false;
                if (arma_S.hp < 0)  arma_S.hp = 0;         
            }

            break;
        }
        case 11:{
            if(defensas.hp > 0) flag = true;
            defensas.hp -= dmg;
            if (defensas.hp <= (defensas.hpMax * 20)/100){
                defensas.func = false;
                if (defensas.hp < 0)  defensas.hp = 0;         
            }

            break;
        }
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
    ener += 50 * generador.rate * deltaT;
    if (ener > enerMax) ener = enerMax;
    if (ener < 0) ener = 0;
}

void NavePj::regenerateHp(Ogre::Real deltaT){
    switch (nanoBots.focus){
        
        case 1: {   //casco
            casco.hp += 40 * nanoBots.rate * deltaT;
            if (casco.hp >= (casco.hpMax * nanoBots.limitMax)/100 ){    //limite de reparacion maximo
                nanoBots.focus = 0;
            }
            if ((casco.hp >= (casco.hpMax * 20)/100) && (casco.func == false)){
                casco.func = true;
                consola->addLinea("Casco en funcionamiento");
                _overlayManager->getOverlayElement("casco")->setMaterialName("cascoN");    //reactivando el funcionamiento
            } 
            break;
        }

        case 2: {   //generador
            generador.hp += 40 * nanoBots.rate * deltaT;
            if (generador.hp >= (generador.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if ((generador.hp >= (generador.hpMax * 20)/100) && (generador.func == false)){
                generador.func = true;
                consola->addLinea("Generador de Energia en funcionamiento");
                _overlayManager->getOverlayElement("gen")->setMaterialName("genN"); 
            }  
            break;
        }
        case 3: {   //ala_I
            ala_I.hp += 40 * nanoBots.rate * deltaT;
            if (ala_I.hp >= (ala_I.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if ((ala_I.hp >= (ala_I.hpMax * 20)/100) && (ala_I.func == false)){
                ala_I.func = true; 
                consola->addLinea("Ala Izquierda en funcionamiento");
                _overlayManager->getOverlayElement("alaIzq")->setMaterialName("alaIzqN");
            }  
            break;
        }
        case 4: {   //ala_D
            ala_D.hp += 40 * nanoBots.rate * deltaT;
            if (ala_D.hp >= (ala_D.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if ((ala_D.hp >= (ala_D.hpMax * 20)/100) && (ala_D.func == false)){
                ala_D.func = true; 
                consola->addLinea("Ala Derecha en funcionamiento");
                _overlayManager->getOverlayElement("alaDer")->setMaterialName("alaDerN");
            }  
            break;
        }
        case 5: {   //propulsor_S
            prop_S.hp += 40 * nanoBots.rate * deltaT;
            if (prop_S.hp >= (prop_S.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if ((prop_S.hp >= (prop_S.hpMax * 20)/100) && (prop_S.func == false)){
                prop_S.func = true; 
                consola->addLinea("Propulsores Secundarios en funcionamiento");
                _overlayManager->getOverlayElement("propS")->setMaterialName("propSN");
            }  
            break;
        }
        case 6: {   //propulsor_P
            prop_P.hp += 40 * nanoBots.rate * deltaT;
            if (prop_P.hp >= (prop_P.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if ((prop_P.hp >= (prop_P.hpMax * 20)/100) && (prop_P.func == false)){
                prop_P.func = true; 
                consola->addLinea("Propulsores Primarios en funcionamiento");
                _overlayManager->getOverlayElement("propP")->setMaterialName("propPN");
            }  
            break;
        }
        case 7: {   //computadora
            computadora.hp += 40 * nanoBots.rate * deltaT;
            if (computadora.hp >= (computadora.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if ((computadora.hp >= (computadora.hpMax * 20)/100) && (computadora.func == false)){
                computadora.func = true; 
                consola->addLinea("Computadora en funcionamiento");
                _overlayManager->getOverlayElement("comp")->setMaterialName("compN");
            }  
            break;
        }
        case 8: {   //sat1
            sat1.hp += 40 * nanoBots.rate * deltaT;
            if (sat1.hp >= (sat1.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if ((sat1.hp >= (sat1.hpMax * 20)/100) && (sat1.func == false)){
                sat1.func = true; 
                consola->addLinea("Satelite de Posicionamiento en funcionamiento");
            }  
            break;
        }
        case 9: {   //sat2
            sat2.hp += 40 * nanoBots.rate * deltaT;
            if (sat2.hp >= (sat2.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if ((sat2.hp >= (sat2.hpMax * 20)/100) && (sat2.func == false)){
                sat2.func = true; 
                consola->addLinea("Satelit de Reconocimiento en funcionamiento");
            }  
            break;
        }
        case 10: {   //arma_S
            arma_S.hp += 40 * nanoBots.rate * deltaT;
            if (arma_S.hp >= (arma_S.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if ((arma_S.hp >= (arma_S.hpMax * 20)/100) && (arma_S.func == false)){
                arma_S.func = true; 
                consola->addLinea("Armamento Secundario en funcionamiento");
            }  
            break;
        }
        case 11: {   //defensas
            defensas.hp += 40 * nanoBots.rate * deltaT;
            if (defensas.hp >= (defensas.hpMax * nanoBots.limitMax)/100 ){    
                nanoBots.focus = 0;
            }
            if ((defensas.hp >= (defensas.hpMax * 20)/100) && (defensas.func == false)){
                defensas.func = true; 
                consola->addLinea("Sistema de Defensa en funcionamiento");
            }  
            break;
        }
    }
}






//calculos por frame:
void NavePj::calculate(Ogre::Real deltaT){
    if (nanoBots.focus != 0) regenerateHp(deltaT);    //reparacion
    if (generador.func) generateEner(deltaT);    //carga de energia
    /*if (nanoBots.focus == 0)*/ nanoBots.focus = setNanoBotsTarget();  //busqueda de reparaciones
    
    
    //propulsores
    naveNodo->translate(Vector3(pCam->getDerivedDirection() * propulsion*2));   //aplicar propulsion

    //rigidBody->forceActivationState ();
    //btVector3 dir = btVector3(pCam->getDerivedDirection().x, pCam->getDerivedDirection().y, pCam->getDerivedDirection().z);
    ///*if ((rigidBody->getBulletRigidBody()->getLinearVelocity()*1) < propulsion *100)*/ rigidBody->getBulletRigidBody()->applyImpulse(btVector3(dir * propulsion * 100 *deltaT),dir);




    
    //rigidBody->getBulletRigidBody()->applyCentralForce(propulsion*pCam->getDerivedDirection().normalisedCopy());
    if (((propulsion/15) < ener) && (prop_P.func == true)){
        ener -= propulsion/15;
    } else {
        propulsion -= 0.2;
        if (propulsion < 0) propulsion = 0;
        consola->addLinea("Energia insuficiente");
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
        if (nanoBots.focus != target) consola->addLinea("Asignando NanoBots de reparacion: Caso Principal");
    } else if (generador.hp < (generador.hpMax * nanoBots.limitMax)/100)
        {
            target = 2;
             if (nanoBots.focus != target) consola->addLinea("Asignando NanoBots de reparacion: Generador de Energia");
        } else if (ala_I.hp < (ala_I.hpMax * nanoBots.limitMax)/100)
            {
                target = 3;
                if (nanoBots.focus != target) consola->addLinea("Asignando NanoBots de reparacion: Ala Izquierda");
            } else if (ala_D.hp < (ala_D.hpMax * nanoBots.limitMax)/100)
                {
                    target = 4;
                    if (nanoBots.focus != target) consola->addLinea("Asignando NanoBots de reparacion: Ala Derecha");
                } else if (prop_P.hp < (prop_P.hpMax * nanoBots.limitMax)/100)
                    {
                        target = 6;
                        if (nanoBots.focus != target) consola->addLinea("Asignando NanoBots de reparacion: Propulsores Primarios");
                    } else if (prop_S.hp < (prop_S.hpMax * nanoBots.limitMax)/100)
                        {
                            target = 5;
                            if (nanoBots.focus != target) consola->addLinea("Asignando NanoBots de reparacion: Propulsores Secundarios");
                        } else if (computadora.hp < (computadora.hpMax * nanoBots.limitMax)/100)
                            {
                                target = 7;
                                if (nanoBots.focus != target) consola->addLinea("Asignando NanoBots de reparacion: Computadora Interna");
                            } else if (defensas.hp < (defensas.hpMax * nanoBots.limitMax)/100)
                                {         
                                    target = 11;
                                    if (nanoBots.focus != target) consola->addLinea("Asignando NanoBots de reparacion: Sistema de Defensa");
                                } else if (sat1.hp < (sat1.hpMax * nanoBots.limitMax)/100)
                                    {
                                        target = 8;
                                        if (nanoBots.focus != target) consola->addLinea("Asignando NanoBots de reparacion: Satelite de Posicionamiento");
                                    } else if (sat2.hp < (sat2.hpMax * nanoBots.limitMax)/100)
                                        {
                                            target = 9;
                                            if (nanoBots.focus != target) consola->addLinea("Asignando NanoBots de reparacion: Satelite de Reconocimiento");
                                        } else if (arma_S.hp < (arma_S.hpMax * nanoBots.limitMax)/100)
                                            {
                                                target = 10;
                                                if (nanoBots.focus != target) consola->addLinea("Asignando NanoBots de reparacion: Armamento Secundario");
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
   

    float factor;
   if (propulsion*0.5 > 1) {
        factor = signo * 0.01 / (propulsion*0.4); 
   } else factor = signo * 0.01;


   if (signo < 1)
   {
       if (ala_I.func){

        naveNodo->yaw(Radian(factor)); 
        //_animState = naveEnt->getAnimationState("rotIzq"); 
        //_animState->setEnabled(true);
        //_animState->loop(false);
    }
   } else if (ala_D.func) naveNodo->yaw(Radian(factor));
   
}

void NavePj::pitch(int signo){               //pitch
    
    
    float factor;
   if (propulsion*0.05 > 1) {
        factor = signo * 0.001 / (propulsion*0.1); 
   } else factor = signo * 0.001;


   // if (signo < 1)
   //{
       if (prop_P.func) naveNodo->pitch(Radian(factor));
   //} else if (prop_P.func) naveNodo->pitch(Radian(signo*0.001));

    //rigidBody->forceActivationState ();
    //rigidBody->getBulletRigidBody()->applyTorqueImpulse(btVector3(signo*300, 0, 0));
    //rigidBody->transform->appendRotation(30, (signo,0,0));
}

void NavePj::roll(int signo){ 
    
    //rigidBody->getBulletRigidBody()->setAngularFactor(900);
    //rigidBody->getBulletRigidBody()->applyTorqueImpulse(btVector3(0,900,0));            //roll
    

    //*anguloGiro = *anguloGiro + btVector3(0,0,propulsion * 100 *signo);
    //if (*anguloGiro->getX().abs() > 0.1) 
    //rigidBody->getBulletRigidBody()->applyTorqueImpulse(*anguloGiro);
   
    //MyMotionState* fallMotionState = new MyMotionState(btTransform(btQuaternion(0,30,0,30),btVector3(56,10,0)),naveNodo);
    //rigidBody->forceActivationState ();
    
    //rigidBody->getBulletRigidBody()->applyTorque(btVector3(0, 0, signo*300));


   float factor;
   if (propulsion*0.5 > 1) {
        factor = signo * 0.001 / (propulsion*0.4); 
   } else factor = signo * 0.005;
   //{
       if (prop_S.func) naveNodo->roll(Radian(factor));
   //} else if (prop_S.func) naveNodo->roll(Radian(signo*0.001));
   


   //naveNodo->roll(Radian(signo*0.001));
   

   //_world->updateTransform();

    //btTransform trans;
    //trans.setRotation(btQuaternion(0,signo,0,10));


    //OgreToBullet(naveNodo,rigidBody->getBulletRigidBody(),trans);
}

void NavePj::Mouse(int x, int y){

    rigidBody->getBulletRigidBody()->applyTorqueImpulse(btVector3(y*0.03, 0, x*0.03));


}







/*
void NavePj::BulletToOgre(btRigidBody* body,btTransform &trans,MyMotionState* motionstate)     
   {
      body->getMotionState()->getWorldTransform(trans);
      mymotionstate->setWorldTransform(trans);
      body->getMotionState()->setWorldTransform(trans);
   }

void NavePj::OgreToBullet(SceneNode* node,btRigidBody* body,btTransform& trans)     
   {
      Vector3 position = node->_getDerivedPosition();
      Quaternion quat = node->_getDerivedOrientation();
      trans.setIdentity();
      trans.setOrigin(btVector3(position.x,position.y,position.z));
      trans.setRotation(btQuaternion(quat.x,quat.y,quat.z,quat.w));
      body->setWorldTransform(trans);
   }*/