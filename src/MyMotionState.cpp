#include "MyMotionState.h"

MyMotionState::MyMotionState(const btTransform &initialpos,Ogre::SceneNode *node)
{
   mVisibleobj = node;
   mPos1 = initialpos;
}

MyMotionState::~MyMotionState()
{

}

void MyMotionState::setNode(Ogre::SceneNode* node)
{
   mVisibleobj = node;
}
void MyMotionState::updateTransform(btTransform& newpos)
{
   mPos1 = newpos;
}
void MyMotionState::getWorldTransform(btTransform& worldTrans )const
{
   worldTrans = mPos1;
}

void MyMotionState::setWorldTransform(const btTransform& worldTrans)
{
   if(NULL == mVisibleobj)return;
   btQuaternion rot = worldTrans.getRotation();
   mVisibleobj->setOrientation(rot.w(),rot.x(),rot.y(),rot.z());
   btVector3 pos = worldTrans.getOrigin();
   mVisibleobj->setPosition(pos.x(),pos.y(),pos.z());
}
