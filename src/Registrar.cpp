#include "Registrar.h"

Registrar::Registrar(double successfulIntersectionRadius) : successfulIntersectionRadius(successfulIntersectionRadius) {}

std::list<Obstacle>::iterator Registrar::CheckIntersection(std::list<Obstacle>& track, glm::vec3 position, double offsetY)
{
    position.y += offsetY;

    double distance = 0;

    for (auto obstacle = track.begin(); obstacle != track.end(); obstacle++)
    {
        distance = glm::length(obstacle->position - position);

        if (distance <= successfulIntersectionRadius)
            return obstacle;
    }

    return track.end();
}
