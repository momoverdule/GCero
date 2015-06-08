#include "PlayState.h"
#include "PauseState.h"
#include "IntroState.h"
#include <iostream>
#include <list>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <GL/gl.h>

#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"  
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"   
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "OgreBulletCollisionsRay.h"
//#include "ListaPuntuaciones.h"


#include "player.h"
#include "planeta.h"
#include "yisus.h"

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btIDebugDraw.h"
#include "MyMotionState.h"

using namespace Ogre;
using namespace std;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;


template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

// ----------------ENTER
void PlayState::enter ()
{
  srand (time(NULL));

  _root = Ogre::Root::getSingletonPtr();
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();

  OIS::ParamList param; 
  size_t windowHandle;  
  ostringstream wHandleStr;

  // creando camaras
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  auxCam = _sceneMgr->createCamera("auxCam");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  
  _sceneMgr->clearScene();


  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
  //_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE); 
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);

  //cam1
  _camera->setPosition(Ogre::Vector3(0,4,-15));
  _camera->lookAt(0,0,0);
  _camera->setNearClipDistance(5);
  _camera->setFarClipDistance(100);
  _camera->setFOVy(Ogre::Degree(52));


  _root->getAutoCreatedWindow()->getCustomAttribute("WINDOW", &windowHandle);
  wHandleStr << windowHandle;
  param.insert(make_pair("WINDOW", wHandleStr.str()));
  
  _inputMgr = OIS::InputManager::createInputSystem(param);
  _keyboard = static_cast<OIS::Keyboard*>(_inputMgr->createInputObject(OIS::OISKeyboard, true));
  
  _inputMgr = OIS::InputManager::createInputSystem(param);
  _keyboard = static_cast<OIS::Keyboard*>
    (_inputMgr->createInputObject(OIS::OISKeyboard, true));

  
  _sceneMgr->setSkyBox(true, "arrakisday",30,2);//SKYDOME
    
  initSDL();
  init();   //inicializacion

  this->_mainTrack->play();


  _exitGame = false;
}



void PlayState::exit ()
{

  cout << "\n Saliendo del juego" << endl;
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
    cout << "\n step1" << endl;

  // Eliminar cuerpos rigidos ------------------------------------
  std::deque <OgreBulletDynamics::RigidBody *>::iterator
  itBody = _bodies.begin();
  while (_bodies.end() != itBody) {
  delete *itBody; ++itBody;
  }
  cout << "\n step2" << endl;
  // Eliminar formas de colision ---------------------------------
  std::deque<OgreBulletCollisions::CollisionShape *>::iterator
  itShape = _shapes.begin();
  while (_shapes.end() != itShape) {
  delete *itShape; ++itShape;
  }
 _bodies.clear(); _shapes.clear();


 
  cout << "\n step3" << endl;
  /*
  // Eliminar cuerpos rigidos (naves)------------------------------------
  std::deque <OgreBulletDynamics::RigidBody *>::iterator
  itNaveBody = _naveBodies.begin();
  while (_naveBodies.end() != itNaveBody) {
  delete *itNaveBody; ++itNaveBody;
  }
  cout << "\n step4" << endl;
  // Eliminar formas de colision (naves)---------------------------------
  std::deque<OgreBulletCollisions::CollisionShape *>::iterator
  itNaveShape = _naveShapes.begin();
  while (_naveShapes.end() != itNaveShape) {
  delete *itNaveShape; ++itNaveShape;
  }
  _naveBodies.clear(); _naveShapes.clear();
  cout << "\n step5" << endl;*/
  // Eliminar planetas ---------------------------------
  std::deque<Planeta *>::iterator
  itPlaneta = _planetas.begin();
  while (_planetas.end() != itPlaneta) {
  delete *itPlaneta; ++itPlaneta;
  }
  cout << "\n step6" << endl;
  _planetas.clear();
  // Eliminar orbitas ---------------------------------
  std::deque<SceneNode *>::iterator
  itOrbita = _orbitas.begin();
  while (_orbitas.end() != itOrbita) {
  delete *itOrbita; ++itOrbita;
  }
  _orbitas.clear();
cout << "\n step7" << endl;




}

void
PlayState::pause()
{
}

void
PlayState::resume()
{
  // Se restaura el background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}


//------------------------------------------FRAME STARTED
bool PlayState::frameStarted(const Ogre::FrameEvent& evt)
{


  
  deltaT = evt.timeSinceLastFrame;
  int fps = 1.0 / deltaT;
  CEGUI::System::getSingleton().injectTimePulse(deltaT);
  
   //actualizar figuras
  player->nave->calculate(deltaT);


  yisus->_world->stepSimulation(deltaT); // Actualizar simulacion Bullet
  //_timeLastObject -= deltaT;



  //movimiento laterales de la nave
  if (keyA_Flag == true)    player->nave->yaw(1);
  if (keyD_Flag == true)    player->nave->yaw(-1);
  //movimiento laterales de la nave
  if (keyW_Flag == true)    player->nave->getNode()->roll(Radian(1*deltaT));
  if (keyS_Flag == true)    player->nave->getNode()->roll(Radian(-1*deltaT));
 


  //player->nave->getRigidBody()->getBulletRigidBody()->updateTransform();

  
  //overlays
  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("hpInfo");
  oe->setCaption("Estado de nave:  " + StringConverter::toString(player->nave->getHp() ));
  oe = _overlayManager->getOverlayElement("enerInfo");
  oe->setCaption("Energia:      " + StringConverter::toString(player->nave->getEner() ));    
  oe = _overlayManager->getOverlayElement("propInfo");
  oe->setCaption("Propulsores:     " + StringConverter::toString(player->nave->getProp() ));
  oe = _overlayManager->getOverlayElement("chatInfo");
  oe->setCaption(player->nave->consola->screen());






 if (rotacion_Flag){
 //rotacion de los planetas
  std::deque <Planeta *>::iterator
  itPlaneta = yisus->_planetas.begin();
  while (yisus->_planetas.end() != itPlaneta) {

    (*itPlaneta)->getNode()->yaw(Radian(((*itPlaneta)->rotacion * deltaT)/5),Node::TS_PARENT);   //rotar
    if ((*itPlaneta)->tipo == 2)(*itPlaneta)->getNode()->getParent()->yaw(Radian(((*itPlaneta)->orbita * deltaT)/5),Node::TS_LOCAL);      //orbitar
    if ((*itPlaneta)->tipo == 1) (*itPlaneta)->getNode()->getParent()->yaw(Radian(((*itPlaneta)->orbita * -deltaT)/2),Node::TS_LOCAL);      //orbitar

    ++itPlaneta;
  }
  }

  /*if (orbita_Flag){
  //orbita de los planetas
  std::deque <SceneNode *>::iterator
  itOrbita = _orbitas.begin();
  while (_orbitas.end() != itOrbita) {

    //(*itOrbita)->
    Vector3 direccion = Vector3(1,0,0);
    if ((*itOrbita)->getPosition().x.fabs() > (*itOrbita)->getPosition().y.fabs()){

        if ((*itOrbita)->getPosition().x.fabs() < (*itOrbita)->getPosition().z.fabs()){
            direccion = Vector3(0,0,1);
        }
    } else {
        direccion = Vector3(0,1,0);
        if ((*itOrbita)->getPosition().y.fabs() < (*itOrbita)->getPosition().z.fabs()){
            direccion = Vector3(0,0,1);
        }
    }*/
    
    //(*itOrbita)->rotate(Quaternion(Ogre::Degree(30*deltaT),(*itOrbita)->getPosition().perpendicular()),Node::TS_PARENT);


    //++itOrbita;
  //}
  //}
 

  return true;
}
//------------------------------------------


//------------------------------------------FRAME ENDED
bool PlayState::frameEnded(const Ogre::FrameEvent& evt)
{

  deltaT = evt.timeSinceLastFrame;
  yisus->_world->stepSimulation(deltaT, 5);  // Actualizar fisica

  if (_exitGame)
    return false;



  
  return true;
}
//------------------------------------------


//------------------------------------------KEY PRESSED
void PlayState::keyPressed(const OIS::KeyEvent &e)
{

  CEGUI::System::getSingleton().injectKeyDown(e.key);
  CEGUI::System::getSingleton().injectChar(e.text);
  // Tecla p --> PauseState.
  if (e.key == OIS::KC_P) {
    pushState(PauseState::getSingletonPtr());
  }

  if (e.key == OIS::KC_1) {
        _viewport->setCamera(_camera);
        Ogre::Overlay *overlay = _overlayManager->getByName("estadoInfo");
        overlay->hide();
        overlay = _overlayManager->getByName("pcChat");
        overlay->hide();
  }


  if (e.key == OIS::KC_2) {
        player->nave->setCamP();
        Ogre::Overlay *overlay = _overlayManager->getByName("estadoInfo");
        overlay->show();
        overlay = _overlayManager->getByName("pcChat");
        overlay->show();  
  }

  if (e.key == OIS::KC_3) {
        player->nave->setCamS1();  
  }

  if (e.key == OIS::KC_4) {
        player->nave->setCamS2();  
  }

  if (e.key == OIS::KC_X) {
    player->nave->aplicarDmg(17);
  } 

  if (e.key == OIS::KC_R) {
    //_animBlender->blend("rotDer", AnimationBlender::Blend, 0.5, false);
    //_animState->setTimePosition(0.0);
    //_animState->setEnabled(true);
    //_animState->setLoop(true);
    _sceneMgr->clearScene();
    init();


  } 



  //Flags de movimiento de la nave
  if (e.key == OIS::KC_A) {     // A
    keyA_Flag = true;
  }
  if (e.key == OIS::KC_D) {     // D
    keyD_Flag = true;
  }
  if (e.key == OIS::KC_W) {     // W
    keyW_Flag = false;
  }
  if (e.key == OIS::KC_S) {     // S
    keyS_Flag = false;
  }

  if (e.key == OIS::KC_O) {     // O
    if (orbita_Flag) {
      orbita_Flag = false;
    }else orbita_Flag = true;
  }
  if (e.key == OIS::KC_R) {     // R
    if (rotacion_Flag) {
      rotacion_Flag = false;
    }else rotacion_Flag = true;
  }


  if(e.key == OIS::KC_C) yisus->_world->setShowDebugShapes (true);   
  if(e.key == OIS::KC_V) yisus->_world->setShowDebugShapes (false); 


}
//------------------------------------------


//------------------------------------------KEY RELASED
void PlayState::keyReleased(const OIS::KeyEvent &e)
{


  if (e.key == OIS::KC_A) {     // A
    keyA_Flag = false;
  }
  if (e.key == OIS::KC_D) {     // D
    keyD_Flag = false;
  }
  if (e.key == OIS::KC_W) {     // W
    keyW_Flag = false;
  }
  if (e.key == OIS::KC_S) {     // S
    keyS_Flag = false;
  }





  if (e.key == OIS::KC_ESCAPE) {

    _exitGame = true;
  }
}
//------------------------------------------


//------------------------------------------MOUSE MOVED
void PlayState::mouseMoved(const OIS::MouseEvent &e)
{

  CEGUI::System::getSingleton().injectMouseMove(e.state.X.rel, e.state.Y.rel); 

  //movimiento de camara
    int posx = e.state.X.rel;   
    int posy = e.state.Y.rel;  
    int posz = e.state.Z.rel;
    //_camera->yaw(Ogre::Radian(-posx*0.0003));
    //_camera->pitch(Ogre::Radian(-posy*0.0003));
   player->nave->roll(-posy);
   player->nave->pitch(posx);
    if (posz != 0)player->nave->setPropulsion(posz);

}
//------------------------------------------

//------------------------------------------MOUSE PRESSED
void PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
  if (id == 1) {
    

    //_pSoundFXManager->load("laserShot.mp3")->play();
    //_simpleEffect = _pSoundFXManager->load("laserShot.mp3");
    //_simpleEffect->play();
  }
  if (id == 0) {
    player->nave->lazerIzq();
    player->nave->lazerDer();
    //player->nave->lazerDer();getSoundFXPtr()->play();
    _pSoundFXManager->load("laserShot.wav")->play();
  }
}
//------------------------------------------


//------------------------------------------MOUSE RELASED
void PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{

  CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));

}

CEGUI::MouseButton PlayState::convertMouseButton(OIS::MouseButtonID id)
{
  CEGUI::MouseButton ceguiId;
  switch(id)
    {
    case OIS::MB_Left:
      ceguiId = CEGUI::LeftButton;
      break;
    case OIS::MB_Right:
      ceguiId = CEGUI::RightButton;
      break;
    case OIS::MB_Middle:
      ceguiId = CEGUI::MiddleButton;
      break;
    default:
      ceguiId = CEGUI::LeftButton;
    }
  return ceguiId;
}

//------------------------------------------

PlayState* PlayState::getSingletonPtr ()
{
return msSingleton;
}

PlayState& PlayState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}



void PlayState::init(){


  _pTrackManager = new TrackManager;
  _pSoundFXManager = new SoundFXManager;

  _mainTrack = _pTrackManager->load("ambient.mp3");
  _simpleEffect = _pSoundFXManager->load("laserShot.wav");



  //overlays
  Ogre::Overlay *overlay = _overlayManager->getByName("estadoInfo");
  overlay->show();
  overlay = _overlayManager->getByName("pcChat");
  overlay->show();
  overlay = _overlayManager->getByName("backCam");
  overlay->show();





  yisus = Yisus::summonYisus();
  yisus->initYisus(_sceneMgr,_bodies,_shapes, _naveBodies, _naveShapes, _planetas, _orbitas);  //inicializando el creador 
  


  //creando el nodo planetas:
  SceneNode* planetaNodo =  _sceneMgr->createSceneNode("planetas");
  _sceneMgr->getRootSceneNode()->addChild(planetaNodo);
  
  
  
   
  
  //_simpleEffect = _pSoundFXManager->load("laserShot.mp3");
  
  // Carga del sonido.
  //_pTrackManager->load("ambient.mp3")->play();   //dale play!!
  

 
  //---------------------------inicializando player
  player = new Player();
  player->nave->setViewPort(_viewport);
  player->nave->getPCam()->setAspectRatio(_camera->getAspectRatio());
  yisus->createNavePj(player->nave->getNode(),player->nave->getEntity(),player->nave);
  //_animBlender = new AnimationBlender(player->nave->getEntity());
  //_animState = player->nave->getEntity()->getAnimationState("rotDer");
  //_animState->setEnabled(false);
  


  keyD_Flag=false;
  keyA_Flag=false;
  keyW_Flag=false;
  keyS_Flag=false;
  rotacion_Flag=true;
  orbita_Flag=true;
  
    

  _camera->setPosition(-16,5,0);
  _camera->setDirection(1,0,0);
  player->nave->getNode()->attachObject(_camera);


  




  yisus->createGalaxy(Vector3(0,0,0),20);  //----------------------- crear sistema solar 
  //yisus->createGalaxy(Vector3(10000,10000,10000),10);



  //--------------------creando camara trasera

  renderer = IntroState::getSingletonPtr()->renderer;

  //Sheet
  CEGUI::Window* sheet = CEGUI::System::getSingleton().getGUISheet();

  CEGUI::Window* ceguiCam = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Ex1/Sheet");
  sheet->addChildWindow(ceguiCam);

  //Render Texture - to - target
  Ogre::TexturePtr tex = _root->getTextureManager()->createManual(
                  "RTT",
                  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                  Ogre::TEX_TYPE_2D,
                  512,
                  512,
                  0,
                  Ogre::PF_R8G8B8,
                  Ogre::TU_RENDERTARGET);

  Ogre::RenderTexture* rtex = tex->getBuffer()->getRenderTarget();



  Ogre::Viewport* v = rtex->addViewport(player->nave->getSat1Cam());
  v->setOverlaysEnabled(false);
  v->setClearEveryFrame(true);
  v->setBackgroundColour(Ogre::ColourValue::Black);


  CEGUI::Texture& guiTex = renderer->createTexture(tex);

  CEGUI::Imageset& imageSet = CEGUI::ImagesetManager::getSingleton().create("RTTImageset", guiTex);

  imageSet.defineImage("RTTImage",
           CEGUI::Point(0.0f,0.0f),
           CEGUI::Size(guiTex.getSize().d_width, guiTex.getSize().d_height),
           CEGUI::Point(0.0f,0.0f));



  CEGUI::Window* ex1 = CEGUI::WindowManager::getSingleton().loadWindowLayout("render.layout");

  //CEGUI::Window* camWin = CEGUI::WindowManager::getSingleton().getWindow("CamWin");
  CEGUI::Window* RTTWindow = CEGUI::WindowManager::getSingleton().getWindow("CamWin/RTTWindow");
  
  RTTWindow->setProperty("Image",CEGUI::PropertyHelper::imageToString(&imageSet.getImage("RTTImage")));

  ceguiCam->addChildWindow(ex1);


  player->nave->setCamP();

  cout << "\n saliendo de init" << endl;  
}



bool PlayState::initSDL () {
    // Inicializando SDL...
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;
    atexit(SDL_Quit);
 
    // Inicializando SDL mixer...
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS, 4096) < 0)
      return false;
 
    // Llamar a Mix_CloseAudio al terminar.
    atexit(Mix_CloseAudio);
 
    return true; 
       
}
