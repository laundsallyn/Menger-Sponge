#include "menger.h"
#include <iostream>

namespace {
	const int kMinLevel = 0;
	const int kMaxLevel = 4;
};

Menger::Menger()
{
	// Add additional initialization if you like
	std::cout << "Menger() default constructor" << std::endl;
}

Menger::~Menger()
{
	std::cout << "~Menger() deconstructor" << std::endl;
}

void
Menger::set_nesting_level(int level)
{
	nesting_level_ = level;
	dirty_ = true;
}

bool
Menger::is_dirty() const
{
	return dirty_;
}

void
Menger::set_clean()
{
	dirty_ = false;
}

// FIXME generate Menger sponge geometry
void
Menger::generate_geometry(std::vector<glm::vec4>& obj_vertices,
                          std::vector<glm::uvec3>& obj_faces) const
{
	/** TODO: 5 points
	 * assembles the vertices and triangles needed 
	 * to draw a cube based on its nesting level.
	 *
	 * This function will determine the bounds of the cube's subcubes
	 * and whether or not to draw a given subcube. Then, if the 
	 * subcube should be drawn, generate_geometry calls internal 
	 * function CreateMenger to append all vertices and triangles 
	 * associated with that subcube. 
	 */

	// Bounds of cube is -0.5 to 0.5

	//TODO: test and remove
	// WARNING: IF VERTICES ARE CHANGED, 
	// INDICES MUST ALSO CHANGE TO MATCH
	obj_vertices.push_back(glm::vec4(-0.5, -0.5, -0.5, 1.0f));
	obj_vertices.push_back(glm::vec4( 0.5, -0.5, -0.5, 1.0f));
	obj_vertices.push_back(glm::vec4(-0.5,  0.5, -0.5, 1.0f));
	obj_vertices.push_back(glm::vec4( 0.5,  0.5, -0.5, 1.0f));

	obj_vertices.push_back(glm::vec4( 0.5, -0.5,  0.5f, -1.0f));
	obj_vertices.push_back(glm::vec4(-0.5, -0.5,  0.5f, -1.0f));
	obj_vertices.push_back(glm::vec4( 0.5,  0.5,  0.5f, -1.0f));
	obj_vertices.push_back(glm::vec4(-0.5, 0.5,  0.5f, -1.0f));

	// face -Z
	obj_faces.push_back(glm::uvec3(0, 1, 2));
	obj_faces.push_back(glm::uvec3(1, 2, 3));

	// face -X
	obj_faces.push_back(glm::uvec3(1, 4, 3));
	obj_faces.push_back(glm::uvec3(4, 6, 3));

	// face +Z
	obj_faces.push_back(glm::uvec3(4, 5, 6));
	obj_faces.push_back(glm::uvec3(5, 7, 6));

	// face +X
	obj_faces.push_back(glm::uvec3(5, 0, 7));
	obj_faces.push_back(glm::uvec3(0, 2, 7));

	// face -Y
	obj_faces.push_back(glm::uvec3(5, 4, 0));
	obj_faces.push_back(glm::uvec3(4, 1, 0));

	// face +Y
	obj_faces.push_back(glm::uvec3(2, 3, 7));
	obj_faces.push_back(glm::uvec3(3, 6, 7));


	 // Determine bound of cube
	 // calculate subcubes
	 // determine which subcubes should be drawn
	 // ifShouldBeDrawn, call CreateMenger on that subcube

}

/** TODO: 20 points
 * Implement CreateMenger to append the vertices and triangles of 
 * subcubes that should be drawn. This should properly position and 
 * scale subcubes as described in pseudocode, then populate vertices 
 * and faces with their values for rendering. You need to ensure 
 * that cube faces (consisting of two triangles) have outward-pointing
 * normals. This means that for every triangle with vertices v_i,v_j,v_k, 
 * the triangle normal (vj-vi) x (vk-vi) must point *away* from the cube.
 * Incorrectly-oriented triangles can be fixed by permuting the order
 * of the triangle's vertex indices. If triangles are not oriented 
 * correctly, they will appear incorrectly shaded (probably black) 
 * when rendered, because they are "inside out." 
 * 
 */
void Menger::CreateMenger(std::vector<glm::vec4>&  obj_vertices, 
              std::vector<glm::uvec3>& obj_faces) const
{
	generate_geometry(obj_vertices, obj_faces);
}

/**
 * Create a square, made of two triangles, to put into vertices
 * and faces
 *
 */
void CreateSquare(std::vector<glm::vec4>&  obj_vertices, 
	              std::vector<glm::uvec3>& obj_faces) 
{
}

class Cube {
public:
	Cube(float min, float max, bool isFull)
			:min_bound(min), max_bound(max) {
		std::cout << "Cube(f,f,b) constructor" << std::endl;
		if (!isFull) {
			//subdivide cube!
			std::cout << "Subdividing cube" << std::endl;
		}

		if (min_bound > max_bound) {
			std::cout << "WTF ARE YOU TRYING TO BUILD A CUBE BACKWARDS?!" << std::endl;
		}

	}

	void generate_vertices() {
		if (children.size() > 0) {
			// have children place themselves, this cube does not
		} else {
			// Full cube, put self
			// 12 vertices!!!
			// Perhaps only 8? Can make use of indices!
			glm::vec4(min_bound, min_bound, min_bound, 1.0f); 
		}
	}

	void putVertices(std::vector<glm::vec4>& vertices,
        			 std::vector<glm::uvec3>& indices) {

	}

protected:
	Cube();
	std::vector<Cube> children;
	float min_bound;
	float max_bound;
};

 /** TODO: 5 points
  * Modify the keyboard callback so that pressing any key from '0' to '4'
  *  generates and displays a Menger sponge of the appropriate level L.
  * The geometry should only be recreated when one of these keys is
  * pressed -- do not procedurally generate the cube every frame! 
  * (Hint: any time you change the vertex or triangle list, you need to
  * inform OpenGL about the new data by binding the vertex and triangle
  * VBOs using glBindBuffer, passing the new data to the GPU using
  * glBufferData, etc.) The skeleton code will do most of this for you,
  * if you slot in your Menger-generation code in the right place. 
  *
  */