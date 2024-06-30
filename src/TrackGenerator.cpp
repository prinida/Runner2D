#include "TrackGenerator.h"

#include <random>

TrackGenerator::TrackGenerator(unsigned int trackCount, unsigned int maximumObstaclesOnOneTrack, double minDistaceBetweenObstacles, double startX, double endX, double startY, double endY) :
    trackCount(trackCount),
    maximumObstaclesOnOneTrack(maximumObstaclesOnOneTrack),
    minDistaceBetweenObstacles(minDistaceBetweenObstacles),
    startX(startX),
    endX(endX),
    startY(startY),
    endY(endY) 
{
    float scaleX = abs(endX - startX) / trackCount;
    float scaleY = abs(endY - startY);

    trackPathScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, 1.0f));
    obstacleScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX / 2, scaleX / 2, 1.0f));

    trackPathPosition.resize(trackCount);

    float x, y = startY, z = 0;
    double step = abs(endX - startX) / trackCount;

    for (int i = 0; i < trackCount; i++)
    {
        x = startX + step * i;
        trackPathPosition[i] = glm::vec3(x, y, z);
    }
}

std::list<Obstacle>& TrackGenerator::GenerateTrackSection(double offsetY)
{
    Obstacle obstacle;
    double x, y, z = 0;
    ObstacleType type;
    double step = abs(endX - startX) / trackCount;
    unsigned int trackObstaclesNumber;

    std::random_device randomDevice;
    int seed = randomDevice();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<> realDistrForY(startY + offsetY, endY + offsetY);
    std::uniform_int_distribution<> intDistrForObstacleType(0, OBSTACLE_TYPES_NUMBER - 1);
    std::uniform_int_distribution<> intDistrForObstacleNumber(0, maximumObstaclesOnOneTrack);

    currentTrack.clear();

    for (int i = 0; i < trackCount; i++)
    {
        x = startX + step * (1 / (double)2 + i - 1 / (double)4);
        trackObstaclesNumber = intDistrForObstacleNumber(generator);

        for (int j = 0; j < trackObstaclesNumber; j++)
        {
            y = realDistrForY(generator);
            type = (ObstacleType)intDistrForObstacleType(generator);

            obstacle.position = glm::vec3(x, y, z);
            obstacle.type = type;
            obstacle.score = scoreObstacles[type];

            if(checkMinDistance(obstacle))
                currentTrack.push_back(obstacle);
        }
    }

    return currentTrack;
}

glm::mat4& TrackGenerator::GetTrackPathScaleMatrix()
{
    return trackPathScaleMatrix;
}

glm::mat4& TrackGenerator::GetObstacleScaleMatrix()
{
    return obstacleScaleMatrix;
}

std::vector<glm::vec3>& TrackGenerator::getTrackPathPosition()
{
    return trackPathPosition;
}

bool TrackGenerator::checkMinDistance(Obstacle& obs)
{
    for (auto& obstacle : currentTrack)
    {
        if (glm::length(obs.position - obstacle.position) < minDistaceBetweenObstacles)
            return false;
    }

    return true;
}
