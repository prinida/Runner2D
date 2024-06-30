#include "Player.h"

#include <algorithm>

Player::Player(unsigned int trackCount, double startX, double endX, double startY, double endY) :
    trackCount(trackCount),
    startX(startX),
    endX(endX),
    startY(startY),
    endY(endY)
{
    unsigned int startPathNumber = trackCount / 2;

    pathWidth = abs(endX - startX) / trackCount;

    currentPosition = glm::vec3(startX + pathWidth * (1 / (float)4 + startPathNumber), abs(endY - startY) * 0.01f, 0.0f);
}

void Player::LeftMove()
{
    double newX = currentPosition.x - pathWidth;
    if (newX >= std::min(startX, endX))
        currentPosition = glm::vec3(newX, currentPosition.y, currentPosition.z);
}

void Player::RightMove()
{
    double newX = currentPosition.x + pathWidth;
    if (newX <= std::max(startX, endX))
        currentPosition = glm::vec3(newX, currentPosition.y, currentPosition.z);
}

glm::vec3 Player::GetPlayerPosition()
{
    return currentPosition;
}
