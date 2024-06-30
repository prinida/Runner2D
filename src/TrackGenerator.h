#ifndef TRACK_GENERATOR_H
#define TRACK_GENERATOR_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <list>
#include <vector>
#include <map>

const unsigned int OBSTACLE_TYPES_NUMBER = 5;

enum ObstacleType
{
    BANANA,
    APPLE,
    ORANGE,
    CLOUD,
    BOMB
};

struct Obstacle
{
    glm::vec3 position;
    ObstacleType type;
    int score;
};

class TrackGenerator
{
public:
    TrackGenerator(unsigned int trackCount, unsigned int maximumObstaclesOnOneTrack, double minDistaceBetweenObstacles, double startX, double endX, double startY, double endY);

    std::list<Obstacle>& GenerateTrackSection(double offsetY = 0);
    glm::mat4& GetTrackPathScaleMatrix();
    glm::mat4& GetObstacleScaleMatrix();
    std::vector<glm::vec3>& getTrackPathPosition();

private:
    bool checkMinDistance(Obstacle& obs);

    unsigned int trackCount;
    unsigned int maximumObstaclesOnOneTrack;
    double startX, endX;
    double startY, endY;
    double minDistaceBetweenObstacles;

    std::list<Obstacle> currentTrack;
    std::vector<glm::vec3> trackPathPosition;
    glm::mat4 trackPathScaleMatrix;
    glm::mat4 obstacleScaleMatrix;

    std::map<ObstacleType, int> scoreObstacles = { {BANANA, 40}, {APPLE, 70}, {ORANGE, 50}, {CLOUD, 100}, {BOMB, -100} }; // инициализация должна быть в другом месте
};

#endif
