#ifndef MyMotionState_H
#define MyMotionState_H

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

using namespace Ogre;

class MyMotionState : public btMotionState
{
public:
   MyMotionState(const btTransform &initialpos,Ogre::SceneNode *node);
   ~MyMotionState();
   void setNode(Ogre::SceneNode* node);
   virtual void getWorldTransform(btTransform& worldTrans )const;
   virtual void setWorldTransform(const btTransform& worldTrans);
   void updateTransform(btTransform& newpos);

protected:
   Ogre::SceneNode* mVisibleobj;
   btTransform mPos1;
};

#endif
