#ifndef MENGER_H
#define MENGER_H

#include <glm/glm.hpp>
#include <vector>

class Cube {
public: 
	Cube(glm::vec4 min, glm::vec4 max, bool isFull);
	void generate_vertices();

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
	void CreateMenger(std::vector<glm::vec4>&  obj_vertices, 
              std::vector<glm::uvec3>& obj_faces) const;
private:
	int nesting_level_ = 0;
	bool dirty_ = false;
};

#endif
