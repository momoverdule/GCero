

#ifndef InputManager_H
#define InputManager_H

#include <Ogre.h>
#include <OIS/OIS.h>

// Gestor para los eventos de entrada (teclado y ratón).
class InputManager : public Ogre::Singleton<InputManager>, public OIS::KeyListener, public OIS::MouseListener {
 public:
  InputManager ();
  virtual ~InputManager ();
  
  void initialise (Ogre::RenderWindow *renderWindow);
  void capture ();

  // Gestión de listeners.
  void addKeyListener (OIS::KeyListener *keyListener,
			 const std::string& instanceName);
  void addMouseListener (OIS::MouseListener *mouseListener,
			   const std::string& instanceName );

  void removeKeyListener (const std::string& instanceName);
  void removeMouseListener (const std::string& instanceName);
  void removeKeyListener (OIS::KeyListener *keyListener);
  void removeMouseListener (OIS::MouseListener *mouseListener);
  
  void removeAllListeners ();
  void removeAllKeyListeners ();
  void removeAllMouseListeners ();
  
  void setWindowExtents (int width, int height);

  OIS::Keyboard* getKeyboard ();
  OIS::Mouse* getMouse ();

  // Heredados de Ogre::Singleton.
  static InputManager& getSingleton ();
  static InputManager* getSingletonPtr ();

 private:
  bool keyPressed (const OIS::KeyEvent &e);
  bool keyReleased (const OIS::KeyEvent &e);
  
  bool mouseMoved (const OIS::MouseEvent &e);
  bool mousePressed (const OIS::MouseEvent &e, 
		     OIS::MouseButtonID id);
  bool mouseReleased (const OIS::MouseEvent &e,
		      OIS::MouseButtonID id);
  
  OIS::InputManager *_inputSystem;
  OIS::Keyboard *_keyboard;
  OIS::Mouse *_mouse;

  std::map<std::string, OIS::KeyListener*> _keyListeners;
  std::map<std::string, OIS::MouseListener*> _mouseListeners;
  std::map<std::string, OIS::KeyListener*>::iterator itKeyListener;
  std::map<std::string, OIS::MouseListener*>::iterator itMouseListener;
  std::map<std::string, OIS::KeyListener*>::iterator itKeyListenerEnd;
  std::map<std::string, OIS::MouseListener*>::iterator itMouseListenerEnd;
};
#endif
