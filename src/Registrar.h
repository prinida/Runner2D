#ifndef REGISTRAR_H
#define REGISTRAR_H

#include "TrackGenerator.h"

#include "glm/glm.hpp"

#include <list>

class Registrar
{
public:
    Registrar(double successfulIntersectionRadius);
    std::list<Obstacle>::iterator CheckIntersection(std::list<Obstacle>& track, glm::vec3 position, double offsetY = 0);

private:
    double successfulIntersectionRadius;
};


#endif
