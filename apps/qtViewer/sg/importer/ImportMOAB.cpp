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
#include "moab/CartVect.hpp"

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
      moab::ErrorCode rval;
      
      rval = mbi->load_mesh(fn.c_str());
      MB_CHK_SET_ERR_CONT(rval, "Could not open model file.");
      
      Ref<sg::TriangleMesh> m = new sg::TriangleMesh;
      Ref<sg::DataVector3f> pos = new sg::DataVector3f;
      Ref<sg::DataVector3f> nor = new sg::DataVector3f;
      Ref<sg::DataVector3i> idx = new sg::DataVector3i;

      std::vector<moab::EntityHandle> tris, verts;
      rval = mbi->get_entities_by_type(0, moab::MBTRI, tris, true);
      MB_CHK_SET_ERR_CONT(rval, "Could not get the model triangles.");

      rval = mbi->get_entities_by_type(0, moab::MBVERTEX, verts, true);
      MB_CHK_SET_ERR_CONT(rval, "Could not get the model vertices.");

      std::cout << "Number of triangles in the model: " << tris.size() << std::endl;
      std::cout << "Number of vertices in the model: " << verts.size() << std::endl;
      pos->v.resize(verts.size());
      // nor->v.resize(verts.size()); only if we want to handle vertex normals
      std::map<moab::EntityHandle, int> index_map;
      for( unsigned int i = 0; i < verts.size(); i++){
	moab::EntityHandle vert = verts[i];
	moab::CartVect coords;
	rval = mbi->get_coords( &vert, 1, coords.array() );
	MB_CHK_SET_ERR_CONT(rval, "Could not get vertex coordinates");
	pos->v[i] = vec3f(coords[0],coords[1],coords[2]);
	index_map[vert] = i;
      }

      for(unsigned int i = 0; i < tris.size(); i++) {
	moab::EntityHandle tri = tris[i];
	moab::EntityHandle const *conn = NULL;
	int len;
	std::vector<moab::EntityHandle> dum_vector;
	rval = mbi->get_connectivity(tri, conn, len, true, &dum_vector);
	MB_CHK_SET_ERR_CONT(rval, "Could not get triangle connectivity");

	vec3i vtx;
	vtx.x = index_map[conn[0]];
	vtx.y = index_map[conn[1]];
	vtx.z = index_map[conn[2]];

	idx->v.push_back(vtx);
      }

      m->index = idx.ptr;
      m->vertex = pos.ptr;
      return m;
    };
    
    void importMOAB(Ref<World> &world, const FileName &fileName)
    {
      std::cout << "ospray::sg::importMOAB: importing from " << fileName << endl;
      MOABLoader loader(world.ptr,fileName);
      Ref<sg::TriangleMesh> mesh = loader.readFile(fileName.str());
      world->node.push_back(mesh.cast<Node>());
    }


  } //end sg
} // end ospray
