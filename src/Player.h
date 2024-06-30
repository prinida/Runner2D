#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>

class Player
{
public:
    Player(unsigned int trackCount, double startX, double endX, double startY, double endY);

    void LeftMove();
    void RightMove();
    glm::vec3 GetPlayerPosition();

private:
    unsigned int trackCount;
    double startX, endX;
    double startY, endY;

    double pathWidth;

    glm::vec3 currentPosition;
};

#endif