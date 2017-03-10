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
    glm::vec3 position ;

    glm::vec3 eye;
    glm::vec3 look;
    glm::vec3 right_vec;
    glm::vec3 center;

    // Initial horizontal angle : toward -Z
    float horizontalAngle = 3.14f / 4;
    // Initial vertical angle : none
    float verticalAngle = 0.0f;
    // Initial Field of View
    float initialFoV = 45.0f;
    glm::vec3 objectCenter = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 up;
    float roll = 0;
    float speed = 10.0f; // 3 units / second
    float mouseSpeed = 0.002f;
    float roll_speed = 0.02f;
    float radius = 5.0f;

public:
    Camera(): eye(0, 0, camera_distance_),
              look(0, 0, -1),
              up(0, 1, 0) {
        right_vec = glm::normalize(glm::cross(up, look));
        center = eye + camera_distance_ * look;
    }
	glm::mat4 get_view_matrix() const{
        return ViewMatrix;
    }
	glm::mat4 get_projection_matrix () const{
        return ProjectionMatrix;
    }
    void FPSComputeMatricesFromInputs(GLFWwindow *, double&, double&);
    void OrbitComputeMatricesFromInputs(GLFWwindow*, double&, double&);
    void computeCameraMatrices();
    void reset(){
         camera_distance_ = 3.0;
         horizontalAngle = 3.14f;
         verticalAngle = 0.0f;
         initialFoV = 45.0f;
    }
};

#endif
