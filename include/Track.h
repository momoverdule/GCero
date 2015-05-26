/*********************************************************************
 * Módulo 1. Curso de Experto en Desarrollo de Videojuegos
 * Autor: David Vallejo Fernández    David.Vallejo@uclm.es
 * Autor: Carlos González Morcillo  Carlos.Gonzalez@uclm.es
 *
 * Código modificado a partir del código de IberOgre
 * http://www.ogre3d.org/tikiwiki/Managing+Game+States+with+OGRE
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/

#ifndef __TRACKH__
#define __TRACKH__

#include <SDL/SDL_mixer.h>
#include <OGRE/Ogre.h>

class Track : public Ogre::Resource {
 public:
  // Constructor (ver Ogre::Resource).
  Track (Ogre::ResourceManager* pManager,
	 const Ogre::String& resource_name,
	 Ogre::ResourceHandle handle,
	 const Ogre::String& resource_group,
	 bool manual_load = false,
	 Ogre::ManualResourceLoader* pLoader = 0);
  ~Track ();

  // Manejo básico del track.
  void play (int loop = -1);
  void pause ();
  void stop ();
  
  void fadeIn (int ms, int loop);
  void fadeOut (int ms);
  static bool isPlaying ();

 private:
  // Funcionalidad de Ogre::Resource.
  void loadImpl ();   
  void unloadImpl (); 
  size_t calculateSize () const;

  // Variables miembro.
  Mix_Music* _pTrack; // SDL
  Ogre::String _path; // Ruta al track.
  size_t _size;       // Tamaño.
};

// Smart pointer a Track.
class TrackPtr: public Ogre::SharedPtr<Track> {
 public:
  // Es necesario implementar constructores y operador de asignación.
 TrackPtr(): Ogre::SharedPtr<Track>() {}
 explicit TrackPtr(Track* m): Ogre::SharedPtr<Track>(m) {}
 TrackPtr(const TrackPtr &m): Ogre::SharedPtr<Track>(m) {}
 TrackPtr(const Ogre::ResourcePtr &r);
 TrackPtr& operator= (const Ogre::ResourcePtr& r);
};

#endif
