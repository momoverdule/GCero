

#ifndef IntroState_H
#define IntroState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

class IntroState : public Ogre::Singleton<IntroState>, public GameState
{
 public:
  IntroState() {}
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
  bool Salir (const CEGUI::EventArgs &evt);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  // Heredados de Ogre::Singleton.
  static IntroState& getSingleton ();
  static IntroState* getSingletonPtr ();
  
  void createCGui();
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
