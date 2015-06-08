#include "IntroState.h"
#include "PlayState.h"
#include <Ogre.h>
#include <string>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUI.h>
#include <iostream>


template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
  _camera = _sceneMgr->createCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));


_root = Ogre::Root::getSingletonPtr();
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();

  OIS::ParamList param; 
  size_t windowHandle;  
  ostringstream wHandleStr;

  // creando camaras
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  //_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  
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










  _sceneMgr->setSkyBox(true, "arrakisday",30,2);//SKYDOME

  createCGui();


  luna = _sceneMgr->createSceneNode("luna");
  _sceneMgr->getRootSceneNode()->addChild(luna);
  Entity* ent = _sceneMgr->createEntity("luna","lunaLuna.mesh");
  luna->attachObject(ent);
  luna->setPosition(150,20,300);
  luna->pitch(Degree(-15));
  luna->yaw(Degree(-75));


  origin = _sceneMgr->createSceneNode();
  origin->setPosition(0,0,0);
  _sceneMgr->getRootSceneNode()->addChild(origin);
  planeta = _sceneMgr->createSceneNode("mont");
  origin->addChild(planeta);
  Entity* ent2 = _sceneMgr->createEntity("planetaMont","planetaMont.mesh");
  planeta->attachObject(ent2);
  planeta->setPosition(0,0,0);
  planeta->translate(-2500,-3020,7300);
  planeta->pitch(Degree(-15));
  planeta->yaw(Degree(-80));



  Ogre::Light* light = _sceneMgr->createLight("lightSol");
  Ogre::SceneNode* nodeL = _sceneMgr->createSceneNode("luces"); 
  nodeL->attachObject(light);
  nodeL->setPosition(-2000,1400,-1000);
  _sceneMgr->getRootSceneNode()->addChild(nodeL);
  _sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  _sceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
  _sceneMgr->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
  _sceneMgr->setShadowTextureCount(2);
  _sceneMgr->setShadowTextureSize(512);
  light->setType(Ogre::Light::LT_POINT);
  light->setSpotlightFalloff(200000.0f);




  _exitGame = false;
}

void
IntroState::exit()
{

  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
IntroState::pause ()
{
}

void
IntroState::resume ()
{
}

bool
IntroState::frameStarted
(const Ogre::FrameEvent& evt) 
{
  float deltaT = evt.timeSinceLastFrame;
  CEGUI::System::getSingleton().injectTimePulse(deltaT);
  
  luna->yaw(Radian(0.1*deltaT)); 
  planeta->yaw(Radian(0.07*deltaT)); 
  origin->yaw(Radian(0.01*deltaT));



  return true;
}

bool IntroState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void IntroState::keyPressed
(const OIS::KeyEvent &e)
{

  CEGUI::System::getSingleton().injectKeyDown(e.key);
  CEGUI::System::getSingleton().injectChar(e.text);
  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) {
    CEGUI::Window* sheet = CEGUI::System::getSingleton().getGUISheet();
    sheet->getChild("menuP")->hide();
    CEGUI::MouseCursor::getSingleton().hide();
    changeState(PlayState::getSingletonPtr());
  }
}

void IntroState::keyReleased
(const OIS::KeyEvent &e )
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void IntroState::mouseMoved
(const OIS::MouseEvent &e)
{
  CEGUI::System::getSingleton().injectMouseMove(e.state.X.rel, e.state.Y.rel); 

}

void
IntroState::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
}

void
IntroState::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
}

CEGUI::MouseButton IntroState::convertMouseButton(OIS::MouseButtonID id)
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

IntroState*
IntroState::getSingletonPtr ()
{
return msSingleton;
}

IntroState&
IntroState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}



void IntroState::createCGui(){
  
  //CEGUI
  renderer = &CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
  CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
  CEGUI::System::getSingleton().setDefaultFont("DejaVuSans-10");
  CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook","MouseArrow");
  CEGUI::MouseCursor::getSingleton().hide();

  CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Sheet");
  
  
  CEGUI::MouseCursor::getSingleton().show();
  
  //MenuPrincipal
  CEGUI::Window* menuP = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","menuP");
  sheet->addChildWindow(menuP);

  //Play button
  CEGUI::Window* jugar = CEGUI::WindowManager::getSingleton().loadWindowLayout("jugarBtn.layout");
  menuP->addChildWindow(jugar);
  CEGUI::Window* jugarBtn = CEGUI::WindowManager::getSingleton().getWindow("menuP/jugar");
  jugarBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::Play,this));
  jugar->addChildWindow(jugarBtn);

  //Load button
  CEGUI::Window* load = CEGUI::WindowManager::getSingleton().loadWindowLayout("loadBtn.layout");
  menuP->addChildWindow(load);

  //Scores button
  CEGUI::Window* scores = CEGUI::WindowManager::getSingleton().loadWindowLayout("scoresBtn.layout");
  menuP->addChildWindow(scores);

  //Instrucciones button
  CEGUI::Window* inst = CEGUI::WindowManager::getSingleton().loadWindowLayout("instruccionesBtn.layout");
  menuP->addChildWindow(inst);

  //Creditos button
  CEGUI::Window* creditos = CEGUI::WindowManager::getSingleton().loadWindowLayout("creditosBtn.layout");
  menuP->addChildWindow(creditos);

  //Salir button
  CEGUI::Window* salir = CEGUI::WindowManager::getSingleton().loadWindowLayout("salirBtn.layout");
  menuP->addChildWindow(salir);
  CEGUI::Window* salirBtn = CEGUI::WindowManager::getSingleton().getWindow("menuP/salir");
  salirBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::Salir,this));
  salir->addChildWindow(salirBtn);

  
  CEGUI::System::getSingleton().setGUISheet(sheet);
}


bool IntroState::Play (const CEGUI::EventArgs &evt){
      
    CEGUI::Window* sheet = CEGUI::System::getSingleton().getGUISheet();
    sheet->getChild("menuP")->hide();
    CEGUI::MouseCursor::getSingleton().hide();
    changeState(PlayState::getSingletonPtr());
     
  return true;
}

bool IntroState::Salir (const CEGUI::EventArgs &evt){
      
      exit();
     
  return true;
}
