#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include "player.h"
#include "planeta.h"
#include "yisus.h"
#include "AnimationBlender.h"

#include "GameState.h"
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUI.h>

#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
 

#include "TrackManager.h"
#include "SoundFXManager.h"


using namespace Ogre;
using namespace std;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
 public:
  PlayState () {}

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  OIS::Keyboard* _keyboard;
  OIS::InputManager* _inputMgr;
  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  void mouseMoved (const OIS::MouseEvent &e);
  void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);
  
  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  // Heredados de Ogre::Singleton.
  static PlayState& getSingleton ();
  static PlayState* getSingletonPtr ();

  void init();
  bool initSDL();

  //sonido
  TrackPtr getTrackPtr () { return _mainTrack; }
  SoundFXPtr getSoundFXPtr () { return _simpleEffect; }



  

  std::list <OgreBulletDynamics::RigidBody *>         _naveBodies;
  std::list <OgreBulletCollisions::CollisionShape *>  _naveShapes;

  std::deque <OgreBulletDynamics::RigidBody *>         _bodies;
  std::deque <OgreBulletCollisions::CollisionShape *>  _shapes;


  //Planetas:
  std::deque <Planeta *> _planetas;
  std::deque <SceneNode *> _orbitas;
  SceneNode* addPlaneta(int tipoPlaneta ,Vector3 pos);
  void createGalaxy(Vector3 pos ,int planetasNum);


  Real deltaT;
  bool keyA_Flag;
  bool keyD_Flag;
  bool keyW_Flag;
  bool keyS_Flag;
  bool orbita_Flag;
  bool rotacion_Flag;


  Player* player;
  Yisus* yisus;
  
  void disparar(Quaternion ori,Vector3 pos,Vector3 dir, int speed);

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::RenderWindow* _win;
  Ogre::Viewport* _viewport;  
  Ogre::OverlayManager* _overlayManager;
  Ogre::AnimationState *_animState;
  AnimationBlender *_animBlender;
  Ogre::Camera* _camera;
  Ogre::Camera* auxCam;
  int _numEntities;

  CEGUI::OgreRenderer* renderer; 

  // Manejadores del sonido.
  TrackManager* _pTrackManager;
  SoundFXManager* _pSoundFXManager;
  TrackPtr _mainTrack;
  SoundFXPtr _simpleEffect;
  




  bool _exitGame;
};

#endif
