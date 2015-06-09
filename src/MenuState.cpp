#include "MenuState.h"
#include "PlayState.h"
#include <Ogre.h>
#include <string>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUI.h>
#include <iostream>


using namespace std;

template<> MenuState* Ogre::Singleton<MenuState>::msSingleton = 0;

void
MenuState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();

  OIS::ParamList param; 
  size_t windowHandle;  
  ostringstream wHandleStr;

  // creando camaras
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
    cout << "camaras creadas\n" << endl;
  _sceneMgr->clearScene();


  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
  //_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE); 
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);
    cout << "viewport listo\n" << endl;

  //cam1
  _camera->setPosition(Ogre::Vector3(0,4,-15));
  _camera->lookAt(0,0,0);
  _camera->setNearClipDistance(5);
  _camera->setFarClipDistance(100);
  _camera->setFOVy(Ogre::Degree(52));
    cout << "seteo de camara\n" << endl;


  _root->getAutoCreatedWindow()->getCustomAttribute("WINDOW", &windowHandle);
  wHandleStr << windowHandle;
  param.insert(make_pair("WINDOW", wHandleStr.str()));






    cout << "menuState, cargas primarias listas\n" << endl;
  createGui();
    cout << "creategui listo\n" << endl;
  changeGui(0);
    cout << "changegui  listo\n" << endl;

  _exitGame = false;
}

void
MenuState::exit()
{

  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
MenuState::pause ()
{
}

void
MenuState::resume ()
{
}

bool
MenuState::frameStarted
(const Ogre::FrameEvent& evt) 
{
  float deltaT = evt.timeSinceLastFrame;
  CEGUI::System::getSingleton().injectTimePulse(deltaT);
  
  switch (estadoMenu){
  	case 0:{
  		luna->yaw(Radian(0.1*deltaT)); 
  		planeta->yaw(Radian(0.07*deltaT)); 
	  	origin->yaw(Radian(0.01*deltaT));
  		break;
 	}
  
  	case 1:{
  		_camera->roll(Radian(0.05*deltaT));
  		break;
  	} 
  
  }

  return true;
}

bool MenuState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void MenuState::keyPressed
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

void MenuState::keyReleased
(const OIS::KeyEvent &e )
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void MenuState::mouseMoved
(const OIS::MouseEvent &e)
{
  CEGUI::System::getSingleton().injectMouseMove(e.state.X.rel, e.state.Y.rel); 

}

void
MenuState::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
}

void
MenuState::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
}

CEGUI::MouseButton MenuState::convertMouseButton(OIS::MouseButtonID id)
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

MenuState*
MenuState::getSingletonPtr ()
{
return msSingleton;
}

MenuState&
MenuState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}





void MenuState::createGui(){

  CEGUI::MouseCursor::getSingleton().show();
  CEGUI::Window* sheet = CEGUI::System::getSingleton().getGUISheet(); 

  //------------------MenuPrincipal
  CEGUI::Window* menuP = sheet->getChild("menuP");

  //Play button
  CEGUI::Window* jugarBtn =  menuP->getChild("menuP/jugar"); 
  jugarBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MenuState::Play,this));

  //Load button;


  //Scores button
  CEGUI::Window* scoreBtn =  menuP->getChild("menuP/scores");
  scoreBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MenuState::Score,this));

  //Instrucciones button
  CEGUI::Window* instBtn =  menuP->getChild("menuP/instrucciones");
  instBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MenuState::Instrucciones,this));


  //Creditos button


  //Salir button
  CEGUI::Window* salirBtn = menuP->getChild("menuP/salir");
  salirBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MenuState::Salir,this));
  
  menuP->hide();
  //-----------------------------


//------------------MenuScore
  CEGUI::Window* menuS = sheet->getChild("menuS");


  //volver button
  CEGUI::Window* scoreVolverBtn = menuS->getChild("menuS/volver");
  scoreVolverBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MenuState::Volver,this));
  
  menuS->hide();
  //-----------------------------


}




void MenuState::changeGui(int estado){

  CEGUI::Window* sheet = CEGUI::System::getSingleton().getGUISheet();  
  _sceneMgr->clearScene();
  sheet->getChild("menuP")->hide();
  sheet->getChild("menuS")->hide();
 
  switch (estado){
  
  case 0:{
  _camera->setPosition(Ogre::Vector3(0,4,-15));
  _camera->lookAt(0,0,0);
  sheet->getChild("menuP")->show();
  _sceneMgr->setSkyBox(true, "arrakisday",30,2);//SKYDOME


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
  
  estadoMenu = 0;
  break;
  }
  
  case 1: {
  	_camera->lookAt(30,10,0);
  	_camera->roll(Degree(110));
  	sheet->getChild("menuS")->show();
  	_sceneMgr->setSkyBox(true, "down",30,2);//SKYDOME
  	estadoMenu = 1;
  	break;
  }
  
  case 2: {
  	_camera->lookAt(70,-8,10);
  	_camera->roll(Degree(25));
  	_camera->pitch(Degree(180));
  	sheet->getChild("menuS")->show();
  	_sceneMgr->setSkyBox(true, "5dim",30,2);//SKYDOME
  	estadoMenu = 2;
  	break;
  }
  
  
  
  
  }
  
  
}


bool MenuState::Play (const CEGUI::EventArgs &evt){
      
     cout << "entro al boton play \n" << endl;
    CEGUI::Window* sheet = CEGUI::System::getSingleton().getGUISheet();
    sheet->getChild("menuP")->hide();
    CEGUI::MouseCursor::getSingleton().hide();
    changeState(PlayState::getSingletonPtr());
     
  return true;
}

bool MenuState::Score (const CEGUI::EventArgs &evt){
      
      changeGui(1);
     
  return true;
}

bool MenuState::Instrucciones (const CEGUI::EventArgs &evt){
      
      changeGui(2);
     
  return true;
}




bool MenuState::Salir (const CEGUI::EventArgs &evt){
      
      exit();
     
  return true;
}

bool MenuState::Volver(const CEGUI::EventArgs &evt){
      
  changeGui(0);
     
  return true;
}

