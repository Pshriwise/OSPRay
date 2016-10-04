#undef NDEBUG

#define WARN_INCLUDE_EMBREE_FILENAME 1

// O_LARGEFILE is a GNU extension.
#ifdef __APPLE__
#define  O_LARGEFILE  0
#endif

// sg
#include "SceneGraph.h"
#include "sg/common/Texture2D.h"
#include "sg/geometry/TriangleMesh.h"
#include <cmath>

// moab
#include "moab/Core.hpp"
#include "moab/Interface.hpp"

namespace ospray {
  namespace sg {
    using std::string;
    using std::cout;
    using std::endl;


    class MOABLoader
    {
    public:
      
      World *world;

      /*! Constructor. */
      MOABLoader(World *world, const FileName& fileName);
      
      /*! Destruction */
      ~MOABLoader();

      Ref<sg::TriangleMesh> readFile(std::string fn);
      
    private:
      
      FileName path;
      
      /*! Geometry buffer. */
      std::vector<vec3f> v;
      std::vector<vec3f> vn;
      std::vector<vec2f> vt;
 
    }; // end MOABLoader

    MOABLoader::~MOABLoader() {}
    
    MOABLoader::MOABLoader(World *world, const FileName &fileName) 
      : world(world),
        path(fileName.path())
    {
      /* open file */
      std::ifstream cin;
      cin.open(fileName.c_str());
      if (!cin.is_open()) {
        std::cerr << "cannot open " << fileName.str() << std::endl;
        return;
      }
    }

    Ref<sg::TriangleMesh> MOABLoader::readFile(std::string fn) {
      moab::Interface *mbi = new moab::Core();
      Ref<sg::TriangleMesh> m;
      return m;
    };
    
    void importMOAB(Ref<World> &world, const FileName &fileName)
    {
      std::cout << "ospray::sg::importMOAB: importing from " << fileName << endl;
      MOABLoader loader(world.ptr,fileName);
      Ref<sg::TriangleMesh> mesh = loader.readFile(fileName.str());
    }


  } //end sg
} // end ospray
