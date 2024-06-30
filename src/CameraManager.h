#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraManager
{
public:
    CameraManager(glm::vec3 position,glm::vec3 front, glm::vec3 up);
    glm::mat4 GetViewMatrix();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
};

#endif
