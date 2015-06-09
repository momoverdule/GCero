#include "IntroState.h"
#include "MenuState.h"
#include <Ogre.h>
#include <string>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUI.h>
#include <iostream>

using namespace std;

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





  createCGui();
  cout << "saliendo de introstate\n" << endl;
  changeState(MenuState::getSingletonPtr());



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
  
  

  
  //-------------MenuPrincipal
  CEGUI::Window* menuP = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","menuP");
  sheet->addChildWindow(menuP);

  //Play button
  CEGUI::Window* jugar = CEGUI::WindowManager::getSingleton().loadWindowLayout("jugarBtn.layout");
  menuP->addChildWindow(jugar);

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
  //--------------------
  
  //----------Scores menu
  CEGUI::Window* menuS = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","menuS");
  sheet->addChildWindow(menuS);
  
  //Salir button
  CEGUI::Window* volver = CEGUI::WindowManager::getSingleton().loadWindowLayout("volverBtn.layout");
  menuS->addChildWindow(volver);
  menuS->hide();
  //-------------
  
  
  
  CEGUI::System::getSingleton().setGUISheet(sheet);
}


