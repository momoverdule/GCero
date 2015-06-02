#include "yisus.h"

#include <iostream>
#include <list>
#include <Ogre.h>

#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"  
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"   
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "OgreBulletCollisionsRay.h"
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btIDebugDraw.h"
#include "MyMotionState.h"


using namespace std;
using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;


Yisus* Yisus::yisus_ = NULL;

void Yisus::initYisus(SceneManager* sceneMgr, std::deque <OgreBulletDynamics::RigidBody *> bodies,std::deque <OgreBulletCollisions::CollisionShape *>  shapes,
            std::list <OgreBulletDynamics::RigidBody *>  naveBodies, std::list <OgreBulletCollisions::CollisionShape *>  naveShapes, std::deque <Planeta *> planetas, 
                std::deque <SceneNode *> orbitas){

  _sceneMgr = sceneMgr;

  //tomando contenedores::
  _shapes = shapes;
  _bodies = bodies;
  _naveShapes = naveShapes;
  _naveBodies = naveBodies;
  _planetas = planetas;
  _orbitas = orbitas;

  _numEntities = 0;

  // Creacion del modulo de debug visual de Bullet ---------------
  _debugDrawer = new OgreBulletCollisions::DebugDrawer();
  _debugDrawer->setDrawWireframe(true); 
  SceneNode *node = _sceneMgr->getRootSceneNode()->
    createChildSceneNode("debugNode", Vector3::ZERO);
  node->attachObject(static_cast<SimpleRenderable*>(_debugDrawer));  
  
  // Creacion del mundo (definicion de los limites y la gravedad)
  AxisAlignedBox worldBounds = AxisAlignedBox
  (Vector3(-30000,-30000,-30000), Vector3(10000,10000,10000));
  Vector3 gravity = Vector3(0, 0, 0);
  _world = new OgreBulletDynamics::DynamicsWorld(_sceneMgr,
    worldBounds, gravity);
  _world->setDebugDrawer (_debugDrawer);
  _world->setShowDebugShapes (false); // Muestra formas debug
	

}


Yisus::Yisus(){

  _sceneMgr = NULL;

  //tomando contenedores::
  //_shapes = NULL;
  //_bodies = NULL;
  //_naveShapes = NULL;
  //_naveBodies = NULL;
  //_planetas = NULL;
  //_orbitas = NULL;

  _numEntities = 0;

  // Creacion del modulo de debug visual de Bullet ---------------
  _debugDrawer = NULL;
  _world = NULL;

}


  void Yisus::createNavePj(SceneNode* node, Entity* entity,NavePj* nave){

  OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = NULL;
  OgreBulletCollisions::CollisionShape *bodyShape = NULL;
  OgreBulletDynamics::RigidBody *rigidBody = NULL;

  trimeshConverter = new OgreBulletCollisions::StaticMeshToShapeConverter(entity);
  bodyShape = trimeshConverter->createTrimesh();//->createConvex();
  delete trimeshConverter;
    

  rigidBody = new OgreBulletDynamics::RigidBody("navePj", _world);
  
  rigidBody->setShape(node, bodyShape
         ,0.1 /*Restitucion*/ 
       ,0.5   /*Friccion */
       ,10  /*Masa*/ 
       ,node->_getDerivedPosition()   /*Posicion inicial*/ 
       ,Quaternion(0,0,0,1) /* Orientacion*/ );

    nave->setRigidBody(rigidBody);
    nave->setNode(rigidBody->getSceneNode());
    
    _naveShapes.push_back(bodyShape);   _naveBodies.push_back(rigidBody);
      
    //rigidBody->enableActiveState ();

    rigidBody->getBulletRigidBody()->setCollisionFlags( rigidBody->getBulletRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    rigidBody->getBulletRigidBody()->setActivationState( DISABLE_DEACTIVATION );
}

Yisus::~Yisus(){

  // Eliminar mundo dinamico y debugDrawer -----------------------
  _world->setDebugDrawer(0);
  delete _world->getDebugDrawer();
  delete _world;


}






//--------------ADD PlANETA
SceneNode* Yisus::addPlaneta(int tipoPlaneta ,Vector3 pos) {

 string mesh;
 float radio;
 radio = 25;
 switch (tipoPlaneta){

  case 0: {
    mesh = "sol.mesh";
    radio = 1850;
    break;
  }
  case 1: {
    mesh = "planetaMont.mesh";
    radio = 950;
    break;
  }
  case 2: {
    mesh = "planetaRock.mesh";
    radio = 850;
    break;
  }
  case 3: {
    mesh = "planetaJungle.mesh";
    radio = 800;
    break;
  }
  case 4: {
    mesh = "planetaGasR.mesh";
    radio = 900;
    break; 
  }
  case 5: {
    mesh = "planetaNubes.mesh";
    radio = 600;
    break;
  }
  case 6: {
    mesh = "planetaLava.mesh";
    break;
  }
  case 7: {
    mesh = "lunaJup.mesh";
    radio = 50;
    break;
  }
  case 8: {
    mesh = "lunaDeath.mesh";
    radio = 50;
    break;
  }
  case 9: {
    mesh = "lunaLuna.mesh";
    radio = 50;
    break;
  }
  case 10: {
    mesh = "lunaChatran.mesh";
    radio = 50;
    break;
  }
 }
  

  Entity *entity = _sceneMgr->createEntity("planeta" + StringConverter::toString(_numEntities),mesh);
  //entity->setQueryFlags(PELOTA);
  SceneNode *node = _sceneMgr->createSceneNode("planeta" + StringConverter::toString(_numEntities));
  node->attachObject(entity);

  if (tipoPlaneta == 4) {
    ParticleSystem* ps = _sceneMgr->createParticleSystem("rS" + StringConverter::toString(_numEntities) ,"redSmoke");  
    node->attachObject(ps);
  }
  

  OgreBulletCollisions::SphereCollisionShape *ballShape = 
    new OgreBulletCollisions::SphereCollisionShape(radio);

  OgreBulletDynamics::RigidBody *rigidBall = new 
    OgreBulletDynamics::RigidBody("planetaRigid" + 
      StringConverter::toString(_numEntities), _world);
  
  
    rigidBall->setShape(node, ballShape,
         0.1 /*Restitucion*/ , 1  /*Friccion */,
         0  /*Masa*/ , pos  /*Posicion inicial*/ ,
         Quaternion(0,0,0,1) /* Orientacion*/ );
  
  // Anadimos los objetos a las deques
  _shapes.push_back(ballShape);   _bodies.push_back(rigidBall); 

  node->setDirection(1,0,0);
  rigidBall->getBulletRigidBody()->setCollisionFlags( rigidBall->getBulletRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  rigidBall->getBulletRigidBody()->setActivationState( DISABLE_DEACTIVATION );

  _numEntities++;
  return node;
}




void Yisus::createGalaxy(Vector3 pos ,int planetasNum){


SceneNode* galaxia = _sceneMgr->createSceneNode();
_sceneMgr->getRootSceneNode()->addChild(galaxia);
galaxia->setPosition(pos);

//crear Sol:
Planeta* sol = new Planeta(addPlaneta(0,pos),0, "Minus"+ StringConverter::toString(_numEntities) );
galaxia->addChild(sol->getNode());
_planetas.push_back(sol); 


Ogre::SceneNode* nodeL = _sceneMgr->createSceneNode("luces"+ StringConverter::toString(_numEntities)); 
Ogre::Light* light = _sceneMgr->createLight("lightSol"+ StringConverter::toString(_numEntities));
nodeL->attachObject(light);
sol->getNode()->addChild(nodeL);

 //particulas del sol
ParticleSystem* ps = _sceneMgr->createParticleSystem("Ps"+ StringConverter::toString(_numEntities),"sol");  
sol->getNode()->attachObject(ps);

SceneNode* aux;

_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
_sceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
_sceneMgr->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
_sceneMgr->setShadowTextureCount(2);
_sceneMgr->setShadowTextureSize(512);
light->setType(Ogre::Light::LT_POINT);
light->setSpotlightFalloff(200000.0f);
light->setCastShadows(true);

int i,j,k;
Planeta* pNodo;
Planeta* pNodo2;
SceneNode* planeta;
int distance = 5500;
int distance2;
Vector3 mezclado;
Vector3 mezclado2;
Vector3 poss;
int factor;

cout << "\n sol creado" << endl;



//creando planetas:
for (i = 0; i < planetasNum; ++i)
{
  

  factor = rand()%6+1;
  
  switch(factor){
    case 1:{ 
      mezclado = Vector3(1,0,0);
      poss = Vector3(distance,0,0);
      break;
    }
    case 2:{ 
      mezclado = Vector3(0,1,0);
      poss = Vector3(0,distance,0);
      break;
    }
    case 3:{ 
      mezclado = Vector3(0,0,1);
      poss = Vector3(0,0,distance);
      break;
    }
    case 4:{ 
      mezclado = Vector3(-1,0,0);
      poss = Vector3(-distance,0,0);
      break;
    }
    case 5:{ 
      mezclado = Vector3(0,-1,0);
      poss = Vector3(0,-distance,0);
      break;
    }
    case 6:{ 
      mezclado = Vector3(0,0,-1);
      poss = Vector3(0,0,-distance);
      break;
    }
  }

  pNodo = new Planeta(addPlaneta(((rand()%5)+1),galaxia->_getDerivedPosition() + poss),1, "Planeta" );
  planeta =  _sceneMgr->createSceneNode();
  galaxia->addChild(planeta);
  planeta->addChild(pNodo->getNode());
  //planeta->setAutoTracking(true,galaxia,Vector3::NEGATIVE_UNIT_Y);
  planeta->setDirection(mezclado);

  
  //pNodo->getNode()->setDirection(mezclado*-1);
  //planeta->setDirection(mezclado*-1);

  planeta->pitch(Degree((rand()%361)));
  planeta->rotate(Quaternion(Ogre::Degree((rand()%361)), (mezclado)),Node::TS_PARENT);

  //creando lunas:
  distance2 = 900;

  k = rand()%5; //cantidad de lunas
  for (j = 0; j < k; ++j)
  {

    factor = rand()%6+1;
  
    switch(factor){
      case 1:{ 
        mezclado2 = Vector3(distance2,0,0);
        break;
      }
      case 2:{ 
       mezclado2 = Vector3(0,distance2,0);
       break;
      }
      case 3:{ 
        mezclado2 = Vector3(0,0,distance2);
        break;
      }
      case 4:{ 
        mezclado2 = Vector3(-1*distance2,0,0);
       break;
      }
      case 5:{ 
       mezclado2 = Vector3(0,-1*distance2,0);
       break;
      }
      case 6:{ 
       mezclado2 = Vector3(0,0,-1*distance2);
       break;
      }
    }
    
    pNodo2 = new Planeta(addPlaneta((rand()%5+6),galaxia->_getDerivedPosition() + poss),2, "luna" );  
    pNodo2->getNode()->setPosition(galaxia->_getDerivedPosition() +pos);
    aux = pNodo->getNode()->createChildSceneNode();
    //aux->setPosition(poss);
    
    
    aux->addChild(pNodo2->getNode());   
    pNodo2->getNode()->setDirection(mezclado2*-1);

    aux->pitch(Degree((rand()%361)));
    aux->rotate(Quaternion(Ogre::Degree((rand()%361)), (mezclado)),Node::TS_PARENT);
    pNodo2->getNode()->translate(mezclado2);
    

  

    //planeta->roll(Degree((rand() %361)),Node::TS_LOCAL);      //desorbitar
    //planeta->setDirection(mezclado*-1);
    distance2 += 200;
    _planetas.push_back(pNodo2); 
    cout << "\n luna creada" << endl;
  }

  distance += 4500;
  pNodo->orbita = pNodo->orbita / ((i*1.5)+1);
  _planetas.push_back(pNodo);
  _orbitas.push_back(planeta);
}
  cout << "\n galaxia creada" << endl;
}



void Yisus::disparar(Quaternion ori,Vector3 pos,Vector3 dir,int speed){

      Entity *entity = _sceneMgr->createEntity("lazer_" + StringConverter::toString(_numEntities), "lazerA.mesh");
      //entity->setQueryFlags(PARED);
        SceneNode *node = _sceneMgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject(entity);
        //node->setOrientation(ori);
        node->setPosition(pos);
        node->setDirection(dir);
        node->setOrientation(ori);
        
        //pos = node->getPosition();
        //node->setDirection(dir);
        //node->setOrientation(ori);
        //node->pitch(Degree(90));
          
        //pos = pos + Vector3(12,-1.7,8.5);
        //Vector3 posAux;
        //posAux = pos + Vector3(11.3,-1.7,8.5);

          OgreBulletCollisions::BoxCollisionShape *boxShape = new OgreBulletCollisions::BoxCollisionShape(Vector3(0.1,0.1,1));
        OgreBulletDynamics::RigidBody *rigidBox = new   OgreBulletDynamics::RigidBody("lazer" + StringConverter::toString(_numEntities), _world);
        rigidBox->setShape(node, boxShape
              ,0.5    /*Restitucion*/ 
            ,1    /*Friccion */
            ,0.4    /*Masa*/ 
            ,pos  /*Posicion inicial*/ 
            ,ori /* Orientacion*/ );
                // Anadimos los objetos a las deques  
                btQuaternion btQ = OgreBtConverter::to(ori);
                //(ori.w,ori.x,ori.y,ori.z);
                //btV->setX(ori.x);
                //btV->setY(ori.y);
                //btV->SetZ(ori.z);
                //btV->SetW(ori.w);
                
                //btVector3 btV2;
                //btV2.setX(13.3);
                //btV2.setY(-1.7);
                //btV2.setZ(10.5);

                rigidBox->setOrientation(btQ);

                 //MyMotionState* fallMotionState = new MyMotionState(btTransform(btQ,btV2),node);
               rigidBox->setLinearVelocity( dir * (900+(speed*50)));
               node->setDirection(dir);
               

      _shapes.push_back(boxShape);   _bodies.push_back(rigidBox);
      _numEntities++; 
}





//sobrecarga de operadores
Yisus Yisus::operator= (const Yisus &yisus2){


  _sceneMgr = yisus2._sceneMgr;
  _shapes = yisus2._shapes;
  _bodies = yisus2._bodies;
  _naveShapes = yisus2._naveShapes;
  _naveBodies = yisus2._naveBodies;
  _planetas = yisus2._planetas;
  _orbitas = yisus2._orbitas;
  _numEntities = yisus2._numEntities;
  _debugDrawer = yisus2._debugDrawer;
  _world = yisus2._world;
}

Yisus* Yisus::summonYisus(){
 { 
  if (!yisus_) yisus_ = new Yisus();
  return yisus_;            }
}


//--------------ADD PlANETA
SceneNode* Yisus::addAsteroide(Vector3 pos) {

 float radio;
 radio = 25;
  

  Entity *entity = _sceneMgr->createEntity("asteoride" + StringConverter::toString(_numEntities),"asteroide.mesh");
  //entity->setQueryFlags(PELOTA);
  SceneNode *node = _sceneMgr->createSceneNode("asteroide" + StringConverter::toString(_numEntities));
  node->attachObject(entity);


  OgreBulletCollisions::SphereCollisionShape *ballShape = 
    new OgreBulletCollisions::SphereCollisionShape(radio);

  OgreBulletDynamics::RigidBody *rigidBall = new 
    OgreBulletDynamics::RigidBody("asteoideRigid" + 
      StringConverter::toString(_numEntities), _world);
  
  
    rigidBall->setShape(node, ballShape,
         0.1 /*Restitucion*/ , 1  /*Friccion */,
         0  /*Masa*/ , pos  /*Posicion inicial*/ ,
         Quaternion(0,0,0,1) /* Orientacion*/ );
  
  // Anadimos los objetos a las deques
  _shapes.push_back(ballShape);   _bodies.push_back(rigidBall); 

  node->setDirection(1,0,0);
  rigidBall->getBulletRigidBody()->setCollisionFlags( rigidBall->getBulletRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  rigidBall->getBulletRigidBody()->setActivationState( DISABLE_DEACTIVATION );

  _numEntities++;
  return node;
}
