#ifndef SETTINGS_H
#define SETTINGS_H

#include <glm/glm.hpp>

namespace Settings
{
    extern const unsigned int NUMBER_PATH_ON_TRACK;
    extern const unsigned int MAXIMUM_OBSTACLES_ON_PATH;
    extern const double MIN_DISTANCE_BETWEEN_OBSTACLES;
    extern const double X_COORD_OF_TRACK_LEFT_BOUND;
    extern const double X_COORD_OF_TRACK_RIGHT_BOUND;
    extern const double Y_COORD_OF_TRACK_BOTTOM_BOUND;
    extern const double Y_COORD_OF_TRACK_TOP_BOUND;

    extern const double SUCCESSFULL_INTERSECTION_DISTANCE;
    extern const double OBSTACLES_SPEED_COEFF_ON_TRACK;

    extern const unsigned int START_SCORE;
    extern const unsigned int SAVED_SCORES_NUMBER;

    extern const glm::vec3 CAMERA_POSITION;
    extern const glm::vec3 CAMERA_FRONT_VECTOR;
    extern const glm::vec3 CAMERA_UP_VECTOR;
}

#endif