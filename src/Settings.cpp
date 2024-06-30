#include "Settings.h"

namespace Settings
{
    const unsigned int NUMBER_PATH_ON_TRACK = 7;
    const unsigned int MAXIMUM_OBSTACLES_ON_PATH = 6;
    const double MIN_DISTANCE_BETWEEN_OBSTACLES = 20;
    const double X_COORD_OF_TRACK_LEFT_BOUND = 0;
    const double X_COORD_OF_TRACK_RIGHT_BOUND = 200;
    const double Y_COORD_OF_TRACK_BOTTOM_BOUND = 0;
    const double Y_COORD_OF_TRACK_TOP_BOUND = 300;

    const double SUCCESSFULL_INTERSECTION_DISTANCE = 10;
    const double OBSTACLES_SPEED_COEFF_ON_TRACK = 30;

    const unsigned int START_SCORE = 0;
    const unsigned int SAVED_SCORES_NUMBER = 6;

    const glm::vec3 CAMERA_POSITION = glm::vec3(200.0f, 150.0f, 300.0f);
    const glm::vec3 CAMERA_FRONT_VECTOR = glm::vec3(0.0f, 0.0f, -1.0f);
    const glm::vec3 CAMERA_UP_VECTOR = glm::vec3(0.0f, 1.0f, 0.0f);
}