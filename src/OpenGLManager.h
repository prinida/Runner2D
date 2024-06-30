#ifndef OPENGL_MANAGER_H
#define OPENGL_MANAGER_H

#include "ResourcesManager.h"
#include "CameraManager.h"
#include "TrackGenerator.h"
#include "Player.h"
#include "Registrar.h"
#include "ScoreInfo.h"
#include "Settings.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <list>

class OpenGLManager
{
public:
    OpenGLManager(const std::string& executablePath);
    ~OpenGLManager();

    void Init(GLFWwindow* window);
    void Display(GLFWwindow* window, double currentTime);

    void RecalculateProjectionMatrix(float aspect);
    Player* GetPlayer();

private:
    void SetupVertices();
    void RenderText(std::string shaderName, std::string fontName, std::string text, float x, float y, float scale, glm::vec3 color);
    void RenderScoreHistory(unsigned int scoresNumber, std::string shaderName, std::string fontName, float x, float y, float scale);
    void RenderTrackObstacles(std::list<Obstacle>& track, double currentTime);
    bool CheckInTrackDomain(Obstacle& obs, double currentTime);

    ResourceManager* resourceManager;
    CameraManager* camera;
    TrackGenerator* trackGenerator;
    Player* player;
    Registrar* registrar;
    ScoreInfo* scoreInfo;

    unsigned int numVAOs = 1;
    unsigned int numVBOs = 4;

    GLuint* vao;
    GLuint* vbo;

    glm::mat4 projectionMatrix; // гюлемхрэ мю нпрнцпютхвеяйсч, рср аеяялшякемн декюрэ оепяоейрхбс
    glm::mat4 modelViewMatrix;

    std::list<Obstacle> track1;
    std::list<Obstacle> track2;

    unsigned int completedTracksNumber = 0;
};

#endif
