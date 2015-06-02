#ifndef NAVEPJ_H_
#define NAVEPJ_H_
#include <Ogre.h>
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"	
#include "Shapes/OgreBulletCollisionsCylinderShape.h"   
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include <OgreBulletDynamicsRigidBody.h>
#include "MyMotionState.h"
#include "chatPc.h"


using namespace std;


struct Generador
{
	int hp;
	int hpMax;
	double rate;
	bool func;
};

struct Casco
{
	int hp;
	int hpMax;
	bool func;
};

struct Ala_D
{
	int hp;
	int hpMax;
	bool func;
};

struct Ala_I
{
	int hp;
	int hpMax;
	bool func;
};

struct Prop_P
{
	int hp;
	int hpMax;
	bool func;
};

struct Prop_S
{
	int hp;
	int hpMax;
	bool func;
};

struct Computadora 
{
	int hp;
	int hpMax;
	bool func;
};

struct Sat1
{
	int hp;
	int hpMax;
	bool func;
};

struct Sat2
{
	int hp;
	int hpMax;
	bool func;
};


struct Arma_S
{
	int hp;
	int hpMax;
	bool func;
};

struct Defensas
{
	int hp;
	int hpMax;
	bool func;
};

struct NanoBots
{	
	double rate;
	int limitMax;
	int focus;
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
	
};





class NavePj{
    
private:
	//stats
	int hp;
	int hpMax;
    double ener;
    int enerMax;
    float propulsion;
    bool takingFire;
    static int numNaves;
   
    //partes de la nave:
    Casco casco;
    Generador generador;
    Ala_I ala_I;
    Ala_D ala_D;
    Prop_S prop_S;
    Prop_P prop_P;
    Computadora computadora;
    Sat1 sat1;
    Sat2 sat2;
    Arma_S arma_S;
    Defensas defensas;
    NanoBots nanoBots;
    

    //funciones relacionadas con el tiempo:
    void generateEner(Ogre::Real deltaT);
    void regenerateHp(Ogre::Real deltaT);
    int setNanoBotsTarget();
    void propulsores();
    int hpActual();

    void BulletToOgre(btRigidBody* body,btTransform &trans,MyMotionState* motionstate);
    void OgreToBullet(SceneNode* node,btRigidBody* body,btTransform& trans);     
     


    //punteros graficos:
    Ogre::SceneNode* naveNodo;
    Ogre::SceneNode* gun2;
    Ogre::SceneNode* gun1;
    Ogre::SceneNode* sat1Nodo;
    Ogre::SceneNode* sat2Nodo;
    OgreBulletDynamics::RigidBody *rigidBody;
    Ogre::Entity* naveEnt;
    Ogre::Camera* pCam;
 	Ogre::Camera* sat1Cam;
 	Ogre::Camera* sat2Cam;
 	Ogre::Viewport* viewP;
 	Ogre::Light* luzDelantera;
 	Ogre::Light* luzProp1;
 	Ogre::Light* luzProp2;
 	Ogre::OverlayManager* _overlayManager;



public:
	NavePj();
    NavePj(int hpNave, int enerMax, Ogre::Vector3 pos);
    ~NavePj();


    //getters
	int getHp();
	Ogre::Vector3 getPosition(); 
	Ogre::Quaternion getDirectionQuad(); 
	Ogre::Entity* getEntity();
	Ogre::SceneNode* getNode();
	OgreBulletDynamics::RigidBody* getRigidBody();
	Ogre::Camera* getSat1Cam();
	Ogre::Camera* getPCam();
	int getEner();
	int getProp();
	int getPropNeta();
	Ogre::Vector3 getDirection();
	Ogre::Vector3 getGun1();
	Ogre::Vector3 getGun2();

	//setters
	void setNanoBotsTarget(int target);
	void setCamP();
	void setCamS1();
	void setCamS2();
	void setViewPort(Ogre::Viewport* port);
	void setRigidBody(OgreBulletDynamics::RigidBody* body);
	void setPropulsion(int signo);
	void setNode(Ogre::SceneNode* nodo);


	//gestores
	//void textoEnPantalla();
	//void gestorMov();
    //void gestorSat();
    //void gestorComputadora();
    void calculate(Ogre::Real deltaT);
    void aplicarDmg(int dmg);
    chatPc *consola;
    Ogre::AnimationState *_animState;


    //mandos del avion
    void yaw(int signo);
    void pitch(int signo);
    void roll(int signo);
    void lateral(int signo);
    void Mouse(int x, int y);
    void lazerDer();
    void lazerIzq();

};

#endif