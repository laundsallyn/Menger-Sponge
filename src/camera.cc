#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
using namespace std;

glm::mat4 computeLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up){
    glm::vec3 const f(glm::normalize(center - eye));
    glm::vec3 const s(glm::normalize(cross(f, up)));
    glm::vec3 const u(glm::cross(s, f));

    glm::mat4 Result(1.0);
    Result[0][0] = s.x;
    Result[1][0] = s.y;
    Result[2][0] = s.z;
    Result[0][1] = u.x;
    Result[1][1] = u.y;
    Result[2][1] = u.z;
    Result[0][2] =-f.x;
    Result[1][2] =-f.y;
    Result[2][2] =-f.z;
    Result[3][0] =-glm::dot(s, eye);
    Result[3][1] =-glm::dot(u, eye);
    Result[3][2] = glm::dot(f, eye);
    return Result;
}



void Camera::FPSComputeMatricesFromInputs(GLFWwindow *window, double &init_x, double &init_y){
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();
    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);


    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        cout<<"@@@@@@@@@@"<<endl;
        // Compute new orientation
        float deltaX = (float)(init_x - xpos);
        float deltaY = (float)(init_y - ypos);

        horizontalAngle += mouseSpeed * deltaX /30;
        verticalAngle += mouseSpeed * deltaY/30;
        init_x = xpos;
        init_y = ypos;

    }else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        if (init_y - ypos < 0.0){
            camera_distance_ -= deltaTime * speed;
            position.z += deltaTime * speed;
        }else if (init_y - ypos > 0.0){
            camera_distance_ += deltaTime * speed;
            position.z -= deltaTime * speed;
        }
        init_x = xpos;
        init_y = ypos;
    }


    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction (
                cos(verticalAngle) * sin(horizontalAngle),
                sin(verticalAngle),
                cos(verticalAngle) * cos(horizontalAngle)
                );

    // Right vector
    glm::vec3 right = glm::vec3(
                sin(horizontalAngle - 3.14f / 2.0f),
                0,
                cos(horizontalAngle - 3.14f / 2.0f)
                );

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }

    float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix = computeLookAt(
                position,           // Camera is here
                position + direction, // and looks here : at the same position, plus "direction"
                up                  // Head is up (set to 0,-1,0 to look upside-down)
                );

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}

void Camera::OrbitComputeMatricesFromInputs(GLFWwindow *window, double &init_x, double &init_y){
    // glfwGetTime is called only once, the first time this function is called
        static double lastTime = glfwGetTime();

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        // Get mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);


//        // Compute new orientation
//        time += mouseSpeed * float(1024 / 2 - xpos);
//        height += sin( mouseSpeed * float(768 / 2 - ypos) );

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            // Compute new orientation
            float deltaX = (float)(init_x - xpos);
            float deltaY = (float)(init_y - ypos);

            horizontalAngle += mouseSpeed * deltaX ;
            verticalAngle += mouseSpeed * deltaY;
            init_x = xpos;
            init_y = ypos;
            cout<<"horizontalAngle = "<< horizontalAngle <<endl;
            cout<<"verticalAngle = "<< verticalAngle <<endl;

        }else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
            if (init_y - ypos < 0.0){
                camera_distance_ -= deltaTime * speed;
                position.z += deltaTime * speed;
            }else if (init_y - ypos > 0.0){
                camera_distance_ += deltaTime * speed;
                position.z -= deltaTime * speed;
            }
            init_x = xpos;
            init_y = ypos;
        }


        // Direction : Spherical coordinates to Cartesian coordinates conversion
        glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
            );

        // Right vector
        glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f / 2.0f),
            0,
            cos(horizontalAngle - 3.14f / 2.0f)
            );

        // Up vector
        glm::vec3 up = glm::cross(right, direction);

        // Move forward
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            radius -= deltaTime * speed;
            if (radius < 0) // don't want radius to be negative
                radius = 0;
        }
        // Move backward
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            radius += deltaTime * speed;
        }
        /*
        // Strafe right
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            position += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            position -= right * deltaTime * speed;
        }
        */

        glm::vec3 objectOffset(radius*cos(horizontalAngle), radius*2, radius* sin(verticalAngle));

        position = objectCenter + objectOffset;

        float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

        ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
        // Camera matrix
        ViewMatrix = computeLookAt(
            position,           // Camera is here
            objectCenter, // and looks here : at the same position, plus "direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
            );

        // For the next frame, the "last time" will be "now"
        lastTime = currentTime;
}
