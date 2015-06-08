
EXEC := G-Cero_Comando

DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/

CXX := g++

# Flags de compilación -----------------------------------------------
CXXFLAGS := -I $(DIRHEA) -Wall `pkg-config --cflags CEGUI-OGRE` `pkg-config	\
--cflags OgreBullet` `pkg-config --cflags bullet`  

# Flags del linker ---------------------------------------------------
LDFLAGS := `pkg-config --libs-only-L CEGUI-OGRE` `pkg-config --libs-only-l	\
OgreBullet` `pkg-config --libs-only-l bullet`

LDLIBS := `pkg-config --libs-only-l CEGUI-OGRE` `pkg-config --libs-only-l	\
OgreBullet` `pkg-config --libs-only-l bullet` -lOIS -lGL -lstdc++ -lSDL -lSDL_mixer -lboost_system -lConvexDecomposition -lboost_graph

# Modo de compilación (-mode=release -mode=debug) --------------------
ifeq ($(mode), release) 
	CXXFLAGS += -O2 -D_RELEASE
else 
	CXXFLAGS += -g -D_DEBUG
	mode := debug
endif

# Obtención automática de la lista de objetos a compilar -------------
OBJS := $(subst $(DIRSRC), $(DIROBJ), \
	$(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))

.PHONY: all clean

all: info $(EXEC)

info:
	@echo '------------------------------------------------------'
	@echo '>>> Using mode $(mode)'
	@echo '    (Please, call "make" with [mode=debug|release])  '
	@echo '------------------------------------------------------'

# Enlazado -----------------------------------------------------------
$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compilación --------------------------------------------------------
$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDLIBS)

# Limpieza de temporales ---------------------------------------------
clean:
	rm -f *.log $(EXEC) *~ $(DIROBJ)* $(DIRSRC)*~ $(DIRHEA)*~ 

edit:
	emacs $(wildcard $(DIRSRC)*.cpp) $(wildcard $(DIRHEA)*.h) &
