#ifndef MyMotionState_H
#define MyMotionState_H

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

using namespace Ogre;

class MyMotionState : public btMotionState {

protected:
	Ogre::SceneNode* _visibleobj;
	btTransform _pos;

public:
	MyMotionState(const btTransform &initialpos,
	Ogre::SceneNode* node);
	virtual ~MyMotionState();
	void setNode(Ogre::SceneNode* node);
	virtual void getWorldTransform(btTransform &worldTr) const;
	virtual void setWorldTransform(const btTransform &worldTr);

};
#endif
