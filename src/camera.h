#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

class Camera {
private:
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    // Initial position : on +Z
    float camera_distance_ = 5.0;
    float deltaTime;
    glm::vec3 position = glm::vec3(0, 0, 5);
    // Initial horizontal angle : toward -Z
    float horizontalAngle = 3.14f;
    // Initial vertical angle : none
    float verticalAngle = 0.0f;
    // Initial Field of View
    float initialFoV = 45.0f;
    glm::vec3 objectCenter = glm::vec3(0.0, 0.0, 0.0);

    float speed = 10.0f; // 3 units / second
    float mouseSpeed = 0.002f;

    float radius = 5.0f;

public:

	glm::mat4 get_view_matrix() const{
        return ViewMatrix;
    }
	glm::mat4 get_projection_matrix () const{
        return ProjectionMatrix;
    }
    void FPSComputeMatricesFromInputs(GLFWwindow *, double&, double&);
    void OrbitComputeMatricesFromInputs(GLFWwindow*, double&, double&);
    void reset(){
         camera_distance_ = 3.0;
         horizontalAngle = 3.14f;
         verticalAngle = 0.0f;
         initialFoV = 45.0f;
    }
};

#endif
