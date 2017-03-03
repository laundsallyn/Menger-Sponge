#ifndef MENGER_H
#define MENGER_H

#include <glm/glm.hpp>
#include <vector>
#include <queue>

class Cube {
public: 
	Cube(glm::vec4 min, glm::vec4 max, bool isFull);
	void print();
	void generate_cube(std::vector<glm::vec4>& obj_vertices,
	                   std::vector<glm::uvec3>& obj_faces, 
	                   int voff);
	glm::vec4& get_min_bound();
	glm::vec4& get_max_bound();

protected:
	Cube();
	glm::vec4 min_bound;
	glm::vec4 max_bound;
};

class Menger {
public:
	Menger();
	~Menger();
	void set_nesting_level(int);
	bool is_dirty() const;
	void set_clean();
	void generate_geometry(std::vector<glm::vec4>& obj_vertices,
	                       std::vector<glm::uvec3>& obj_faces) const;
private:
	void CreateMenger(std::vector<glm::vec4>&  obj_vertices, 
              		  std::vector<glm::uvec3>& obj_faces, 
              		  std::queue<Cube>&        cube_list) const;
	int nesting_level_ = 0;
	bool dirty_ = false;
};

#endif
