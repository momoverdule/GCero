#ifndef YISUS_H_
#define YISUS_H_
#include <Ogre.h>
#include "planeta.h"
#include "player.h"
#include "navePj.h"
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"  
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"   
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "OgreBulletCollisionsRay.h"
#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>

using namespace Ogre;
using namespace std;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;


class Yisus{    //(singleton)
    
private:
    Yisus();
    static Yisus* yisus_;

    

    SceneManager* _sceneMgr;

    //sobrecarga de operadores:
    Yisus operator= (const Yisus &yisus2);

    



public:
    
    ~Yisus();
     static Yisus* summonYisus();

     void initYisus(SceneManager* _sceneMgr, std::deque <OgreBulletDynamics::RigidBody *> bodies ,std::deque <OgreBulletCollisions::CollisionShape *>  shapes,
            std::list <OgreBulletDynamics::RigidBody *>  naveBodies, std::list <OgreBulletCollisions::CollisionShape *>  naveShapes, std::deque <Planeta *> planetas, 
                std::deque <SceneNode *> orbitas);


    void createNavePj(SceneNode* node, Entity* entity, NavePj* nave);
    SceneNode* addPlaneta(int tipoPlaneta ,Vector3 pos);
    SceneNode* addAsteroide(Vector3 pos);
    void createGalaxy(Vector3 pos ,int planetasNum);
    void disparar(Quaternion ori,Vector3 pos,Vector3 dir,int speed);
    int _numEntities;

    OgreBulletDynamics::DynamicsWorld * _world;
    OgreBulletCollisions::DebugDrawer * _debugDrawer;


    std::deque <OgreBulletDynamics::RigidBody *>         _bodies;
    std::deque <OgreBulletCollisions::CollisionShape *>  _shapes;

    std::list <OgreBulletDynamics::RigidBody *>         _naveBodies;
    std::list <OgreBulletCollisions::CollisionShape *>  _naveShapes;

    std::deque <Planeta *> _planetas;
    std::deque <SceneNode *> _orbitas;


};

#endif