#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <GLFW/glfw3.h>


class Camera {
public:

	glm::mat4 get_view_matrix() const{
        return ViewMatrix;
    }
	glm::mat4 get_projection_matrix () const{
        return ProjectionMatrix;
    }
    void computeMatricesFromInputs(GLFWwindow *);
    void zoomIn(){camera_distance_ -= zoomSpeed;}
    void zoomOut(){camera_distance_ += zoomSpeed;}

	// FIXME: add functions to manipulate camera objects.
private:
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
    // Initial position : on +Z
    float camera_distance_ = 3.0;

    glm::vec3 position = glm::vec3(0, 0, camera_distance_);
    // Initial horizontal angle : toward -Z
    float horizontalAngle = 3.14f;
    // Initial vertical angle : none
    float verticalAngle = 0.0f;
    // Initial Field of View
    float initialFoV = 45.0f;

    float speed = 3.0f; // 3 units / second
    float mouseSpeed = 0.002f;
    float zoomSpeed = 10.2f;
//	glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
//	glm::vec3 up_ = glm::vec3(0.0f, 1.0, 0.0f);
//	glm::vec3 eye_ = glm::vec3(0.0f, 0.0f, camera_distance_);
//	// Note: you may need additional member variables

};

#endif
