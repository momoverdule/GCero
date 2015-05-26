#include "MyMotionState.h"

MyMotionState::MyMotionState(const btTransform &initialpos, Ogre::SceneNode *node){
  _visibleobj = node;
  _pos = initialpos;
}

MyMotionState::~MyMotionState(){ }

void MyMotionState::setNode(Ogre::SceneNode *node) {
  _visibleobj = node;
}
  
void MyMotionState::getWorldTransform(btTransform &worldTrans) const {
  worldTrans = _pos;
}
  
void MyMotionState::setWorldTransform(const btTransform &worldTrans) {
  if(NULL == _visibleobj) return; // Si no hay nodo, return 
  btQuaternion rot = worldTrans.getRotation();
  _visibleobj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
  btVector3 pos = worldTrans.getOrigin();
  _visibleobj->setPosition(pos.x(), pos.y(), pos.z());
}
