#include <SDL/SDL_mixer.h>

#include "SoundFXManager.h"

template<> SoundFXManager* Ogre::Singleton<SoundFXManager>::msSingleton = 0;

int SoundFXManager::_numChannels = 32;
        
SoundFXManager::SoundFXManager()
{
  // Tipo de recurso.
  mResourceType = "SoundFX";
  // Prioridad.
  mLoadOrder = 30.f;
  // Registro del gestor en el sistema.
  Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
  // Reserva de 32 canales de audio.
  Mix_AllocateChannels(_numChannels);
}

SoundFXManager::~SoundFXManager()
{
  // Eliminación del registro.
  Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

SoundFXPtr
SoundFXManager::load
(const Ogre::String& name, const Ogre::String& group)
{
  // Obtención del recurso por nombre...
  SoundFXPtr soundFXPtr = getByName(name);

  // Si no ha sido creado, se crea.
  if (soundFXPtr.isNull())
    soundFXPtr = create(name, group);
  
  // Carga explícita del recurso.
  soundFXPtr->load();

  return soundFXPtr;
}

SoundFXManager&
SoundFXManager::getSingleton()
{
  assert(msSingleton);
  return (*msSingleton);
}

SoundFXManager*
SoundFXManager::getSingletonPtr()
{
  assert(msSingleton);
  return msSingleton;
}

int
SoundFXManager::getAvailableChannels()
{
  return _numChannels;
}

// Creación de un nuevo recurso.
// No se llama a load().
Ogre::Resource*
SoundFXManager::createImpl
(const Ogre::String& name,
 Ogre::ResourceHandle handle,
 const Ogre::String& group,
 bool isManual,
 Ogre::ManualResourceLoader* loader,
 const Ogre::NameValuePairList* createParams)
{
  return new SoundFX(this, name, handle, group, isManual, loader);
}

