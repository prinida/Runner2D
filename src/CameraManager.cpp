#include "CameraManager.h"

CameraManager::CameraManager(glm::vec3 position, glm::vec3 front, glm::vec3 up) :
    position(position),
    front(front),
    up(up)
{
    right = glm::normalize(glm::cross(front, up));
}

glm::mat4 CameraManager::GetViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}
