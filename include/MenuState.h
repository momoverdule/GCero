

#ifndef MenuState_H
#define MenuState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

class MenuState : public Ogre::Singleton<MenuState>, public GameState
{
 public:
  MenuState() {}
  CEGUI::OgreRenderer* renderer;

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  void mouseMoved (const OIS::MouseEvent &e);
  void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);

  bool Play (const CEGUI::EventArgs &evt);
  bool Score (const CEGUI::EventArgs &evt);
  bool Instrucciones (const CEGUI::EventArgs &evt);
  bool Salir (const CEGUI::EventArgs &evt);
  bool Volver (const CEGUI::EventArgs &evt);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  // Heredados de Ogre::Singleton.
  static MenuState& getSingleton ();
  static MenuState* getSingletonPtr ();
  
  void createGui();
  void changeGui(int estado);
  int estadoMenu;
  Ogre::SceneNode* luna;
  Ogre::SceneNode* planeta;
  Ogre::SceneNode* origin;
  
 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  //CEGUI::OgreRenderer* renderer;
  Ogre::OverlayManager* _overlayManager;

  bool _exitGame;
};

#endif
