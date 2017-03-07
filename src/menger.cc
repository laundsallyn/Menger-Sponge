#include "menger.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace {
	const int kMinLevel = 0;
	const int kMaxLevel = 4;
};

void create_subcubes(Cube* cubeptr, std::queue<Cube>& cube_list);

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

	 obj_vertices.clear();
	 obj_faces.clear();

	float min_bound = -0.5; // Bounds of whole cube is -0.5 to 0.5
	float max_bound = 0.5;
	std::queue<Cube> cube_list; //FIFO queue, push cubes as wanted
	std::queue<Cube> subcubes;

	// Start with a cube
	Cube start = Cube(glm::vec4(min_bound, min_bound, min_bound, 1.0f),
	 		   		  glm::vec4(max_bound, max_bound, max_bound, 1.0f),
	 		   		  true
	);
	cube_list.push(start);

	Cube *cubeptr;
	for (int i = 0; i < nesting_level_; ++i) {
		//for each cube (at this level)
		while (!cube_list.empty()) {
			cubeptr = &(cube_list.front());
			// create subcubes from this cube
			// add (drawn) subcubes to queue
			create_subcubes(cubeptr, subcubes);
			// pop "parent" cube
			cube_list.pop();
		}
		swap(cube_list, subcubes);
	}

	// queue should be filled with subcubes.
	// have subcubes add into these vertices
	// int vertex_count = 0;
	// while (!cube_list.empty()) {
	// 	// CreateMenger(obj_vertices, obj_faces);
	// 	vertex_count += 8;
	// 	cube_list.pop();
	// }
	std::cout << "gen_geom(): # of cubes = " << cube_list.size() << std::endl;
	CreateMenger(obj_vertices, obj_faces, cube_list);
	
}

void create_subcubes(Cube* cubeptr, std::queue<Cube>& cube_list) {
	// cube_list should increase by 20 cubes
	// std::vector<Cube> sub;
	int c = cube_list.size();

	glm::vec4 min = cubeptr->get_min_bound();
	glm::vec4 max = cubeptr->get_max_bound();
	float x_off = (max[0] - min[0]) / 3; //should be same for all
	float y_off = (max[1] - min[1]) / 3; //since cube, but for now
	float z_off = (max[2] - min[2]) / 3; //calc each as separate
	
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				int count = 1 + k + j*3 + i*9;
				if (count == 5 || count == 11 || count == 13 || count == 14
					|| count == 15 || count == 17 || count == 23) {
					// don't add
					// TODO: Find a better way...
					continue;
				}

				// TODO: comment out
				// DEBUG: cubes sharing verts?
				// if ( count < 4 || count > 6) {
				// 	continue;
				// }

				glm::vec4 submin = min + glm::vec4(x_off*k, y_off*j, z_off*i, 0.0f);
				glm::vec4 submax = min + glm::vec4(x_off*(k+1), y_off*(j+1), z_off*(i+1), 0.0f);
				Cube small = Cube(submin, submax, true);
				cube_list.push(small);
				// std::cout << "number of subcubes: " << cube_list.size() << std::endl;
			}
		}
	}
}

void Menger::create_floor(std::vector<glm::vec4> &vertices, std::vector<glm::uvec3> &faces){
    vertices.push_back(glm::vec4 (0.f,0.f,0.f,1.f));
    vertices.push_back(glm::vec4 (1.f,0.f,0.f,0.f));
    vertices.push_back(glm::vec4 (0.f,0.f,1.f,0.f));
    vertices.push_back(glm::vec4 (-1.f,0.f,0.f,0.f));
    vertices.push_back(glm::vec4 (0.f,0.f,-1.f,0.f));
    faces.push_back(glm::uvec3(0,1,2));
    faces.push_back(glm::uvec3(0,2,3));
    faces.push_back(glm::uvec3(0,3,4));
    faces.push_back(glm::uvec3(0,4,1));
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
              std::vector<glm::uvec3>& obj_faces,
              std::queue<Cube>& cube_list) const
{
	// std::cout << "CreateMenger(): # of cubes = " << cube_list.size() << std::endl;

	Cube *cubeptr;
	while (!cube_list.empty()) {
		cubeptr = &(cube_list.front());
		cubeptr->generate_cube(obj_vertices, obj_faces);
		// std::cout << "obj_vert size" << obj_vertices.size() << std::endl;
		cube_list.pop();
	}
	std::cout << "CreateMenger(): # of vertices = " << obj_vertices.size() << std::endl;
	std::cout << "CreateMenger(): # of faces    = " << obj_faces.size() << std::endl;
}

Cube::Cube(glm::vec4 min, glm::vec4 max, bool isFull)
			:min_bound(min), max_bound(max) {
	// std::cout << "Cube(vec4,vec4,b) constructor" << std::endl;
	// print();
}

void Cube::generate_cube(std::vector<glm::vec4>& obj_vertices,
	                   std::vector<glm::uvec3>& obj_faces ) {
	glm::vec4& min = get_min_bound();
	glm::vec4& max = get_max_bound();

	int voff = obj_vertices.size();

	// obj_vertices.push_back(glm::vec4(-0.5, -0.5, -0.5, 1.0f));
	// obj_vertices.push_back(glm::vec4( 0.5, -0.5, -0.5, 1.0f));
	// obj_vertices.push_back(glm::vec4(-0.5,  0.5, -0.5, 1.0f));
	// obj_vertices.push_back(glm::vec4( 0.5,  0.5, -0.5, 1.0f));
	obj_vertices.push_back(glm::vec4(min[0], min[1], min[2], 1.0f));
	obj_vertices.push_back(glm::vec4(max[0], min[1], min[2], 1.0f));
	obj_vertices.push_back(glm::vec4(min[0], max[1], min[2], 1.0f));
	obj_vertices.push_back(glm::vec4(max[0], max[1], min[2], 1.0f));

	// obj_vertices.push_back(glm::vec4( 0.5, -0.5,  0.5f, 1.0f));
	// obj_vertices.push_back(glm::vec4(-0.5, -0.5,  0.5f, 1.0f));
	// obj_vertices.push_back(glm::vec4( 0.5,  0.5,  0.5f, 1.0f));
	// obj_vertices.push_back(glm::vec4(-0.5,  0.5,  0.5f, 1.0f));
	obj_vertices.push_back(glm::vec4(max[0], min[1], max[2], 1.0f));
	obj_vertices.push_back(glm::vec4(min[0], min[1], max[2], 1.0f));
	obj_vertices.push_back(glm::vec4(max[0], max[1], max[2], 1.0f));
	obj_vertices.push_back(glm::vec4(min[0], max[1], max[2], 1.0f));

	// face -Z
	// std::cout << "-Z" << std::endl;
	// glm::dvec3 v0(obj_vertices[voff + 0]);
	// glm::dvec3 v1(obj_vertices[voff + 1]);
	// glm::dvec3 v2(obj_vertices[voff + 2]);
	// glm::dvec3 v3(obj_vertices[voff + 3]);
	// glm::dvec3 v4(obj_vertices[voff + 4]);
	// glm::dvec3 v5(obj_vertices[voff + 5]);
	// glm::dvec3 v6(obj_vertices[voff + 6]);
	// glm::dvec3 v7(obj_vertices[voff + 7]);

	// face -Z
	// glm::dvec3 face1 = glm::cross(v1 - v3, v2 - v3);
	// glm::dvec3 face2 = glm::cross(v2 - v0, v1 - v0);
	// std::cout << "  " << glm::to_string(face1) << std::endl;
	// std::cout << "  " << glm::to_string(face2) << std::endl;
	obj_faces.push_back(glm::uvec3(voff + 0, voff + 1, voff + 2));
	obj_faces.push_back(glm::uvec3(voff + 3, voff + 2, voff + 1));

	// face -X
	// std::cout << "-X" << std::endl;
	// face1 = glm::cross(v4 - v1, v3 - v1);
	// face2 = glm::cross(v3 - v6, v4 - v6);
	// std::cout << "  " << glm::to_string(face1) << std::endl;
	// std::cout << "  " << glm::to_string(face2) << std::endl;
	obj_faces.push_back(glm::uvec3(voff + 1, voff + 4, voff + 3));
	obj_faces.push_back(glm::uvec3(voff + 6, voff + 3, voff + 4));

	// face +Z
	// std::cout << "+Z" << std::endl;
	// face1 = glm::cross(v6 - v4, v5 - v4);
	// face2 = glm::cross(v5 - v7, v6 - v7);
	// std::cout << "  " << glm::to_string(face1) << std::endl;
	// std::cout << "  " << glm::to_string(face2) << std::endl;
	obj_faces.push_back(glm::uvec3(voff + 5, voff + 4, voff + 7));
	obj_faces.push_back(glm::uvec3(voff + 6, voff + 7, voff + 4));

	// face +X
	// std::cout << "+X" << std::endl;
	// face1 = glm::cross(v5 - v0, v2 - v0);
	// face2 = glm::cross(v2 - v0, v0 - v7);
	// std::cout << "  " << glm::to_string(face1) << std::endl;
	// std::cout << "  " << glm::to_string(face2) << std::endl;
	obj_faces.push_back(glm::uvec3(voff + 0, voff + 5, voff + 2));
	obj_faces.push_back(glm::uvec3(voff + 7, voff + 2, voff + 5));

	// face -Y
	// std::cout << "-Y" << std::endl;
	// face1 = glm::cross(v2 - v0, v1 - v0);
	// face2 = glm::cross(v2 - v1, v3 - v1);
	// std::cout << "  " << glm::to_string(face1) << std::endl;
	// std::cout << "  " << glm::to_string(face2) << std::endl;
	obj_faces.push_back(glm::uvec3(voff + 0, voff + 1, voff + 5));
	obj_faces.push_back(glm::uvec3(voff + 4, voff + 5, voff + 1));

	// face +Y
	// std::cout << "+Y" << std::endl;
	// face1 = glm::cross(v2 - v0, v1 - v0);
	// face2 = glm::cross(v2 - v1, v3 - v1);
	// std::cout << "  " << glm::to_string(face1) << std::endl;
	// std::cout << "  " << glm::to_string(face2) << std::endl;
	obj_faces.push_back(glm::uvec3(voff + 2, voff + 3, voff + 7));
	obj_faces.push_back(glm::uvec3(voff + 6, voff + 7, voff + 3));
}



void Cube::print() {
	std::cout << "Cube print!" << std::endl;
	std::cout << "  " << glm::to_string(min_bound) << std::endl;
	std::cout << "  " << glm::to_string(max_bound) << std::endl;
}

glm::vec4& Cube::get_min_bound() {
	return min_bound;
}

glm::vec4& Cube::get_max_bound() {
	return max_bound;
}

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
