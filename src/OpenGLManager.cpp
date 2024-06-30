#include "OpenGLManager.h"

#include <iostream>

OpenGLManager::OpenGLManager(const std::string& executablePath)
{
    resourceManager = new ResourceManager(executablePath);
    camera = new CameraManager
    (
        Settings::CAMERA_POSITION, 
        Settings::CAMERA_FRONT_VECTOR, 
        Settings::CAMERA_UP_VECTOR
    );
    trackGenerator = new TrackGenerator
    (
        Settings::NUMBER_PATH_ON_TRACK, 
        Settings::MAXIMUM_OBSTACLES_ON_PATH, 
        Settings::MIN_DISTANCE_BETWEEN_OBSTACLES, 
        Settings::X_COORD_OF_TRACK_LEFT_BOUND,
        Settings::X_COORD_OF_TRACK_RIGHT_BOUND,
        Settings::Y_COORD_OF_TRACK_BOTTOM_BOUND,
        Settings::Y_COORD_OF_TRACK_TOP_BOUND
    );
    player = new Player
    (
        Settings::NUMBER_PATH_ON_TRACK,
        Settings::X_COORD_OF_TRACK_LEFT_BOUND,
        Settings::X_COORD_OF_TRACK_RIGHT_BOUND,
        Settings::Y_COORD_OF_TRACK_BOTTOM_BOUND,
        Settings::Y_COORD_OF_TRACK_TOP_BOUND
    );
    registrar = new Registrar(Settings::SUCCESSFULL_INTERSECTION_DISTANCE);
    scoreInfo = new ScoreInfo(Settings::START_SCORE, Settings::SAVED_SCORES_NUMBER);

    vao = new GLuint[numVAOs];
    vbo = new GLuint[numVBOs];

    projectionMatrix = glm::mat4(1.0f);
    modelViewMatrix = glm::mat4(1.0f);
}

OpenGLManager::~OpenGLManager()
{
    delete resourceManager;
    delete camera;
    delete trackGenerator;
    delete player;
    delete registrar;
    delete scoreInfo;

    delete[] vao;
    delete[] vbo;
}

void OpenGLManager::Init(GLFWwindow* window)
{
    if (!resourceManager->LoadShaders("DefaultShader", "res/shaders/vertShader.glsl", "res/shaders/fragShader.glsl"))
        std::cerr << "Cant't create shader program: " << "DefaultShader" << std::endl;

    if (!resourceManager->LoadShaders("TextShader", "res/shaders/vertShader.glsl", "res/shaders/fragShaderForText.glsl"))
        std::cerr << "Cant't create shader program: " << "TextShader" << std::endl;

    resourceManager->LoadTexture("CobblesPath", "res/textures/cobblespath.png");
    resourceManager->LoadTexture("Apple", "res/textures/apple.png");
    resourceManager->LoadTexture("Cloud", "res/textures/cloud.png");
    resourceManager->LoadTexture("Banana", "res/textures/banana.png");
    resourceManager->LoadTexture("Orange", "res/textures/orange.png");
    resourceManager->LoadTexture("Bomb", "res/textures/bomb.png");
    resourceManager->LoadTexture("Player", "res/textures/player.png");

    resourceManager->LoadFont("Arial", "res/fonts/arial.ttf");

    SetupVertices();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float)width / (float)height;
    projectionMatrix = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // Á‡ÏÂÌËÚ¸ Ì‡ ÓÚÓ„‡ÙË˜ÂÒÍÛ˛ ÔÓÂÍˆË˛

    track1 = trackGenerator->GenerateTrackSection();
    track2 = trackGenerator->GenerateTrackSection(Settings::Y_COORD_OF_TRACK_TOP_BOUND);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLManager::Display(GLFWwindow* window, double currentTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glBindVertexArray(vao[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    resourceManager->GetShaderProgram("DefaultShader")->use();

    resourceManager->GetShaderProgram("DefaultShader")->setMat4("proj_matrix", projectionMatrix);
    resourceManager->GetShaderProgram("DefaultShader")->setMat4("scale_matrix", trackGenerator->GetTrackPathScaleMatrix());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, resourceManager->GetTexture("CobblesPath")->getID());

    for (auto path : trackGenerator->getTrackPathPosition())
    {
        modelViewMatrix = camera->GetViewMatrix();
        modelViewMatrix *= glm::translate(glm::mat4(1.0f), path);
        resourceManager->GetShaderProgram("DefaultShader")->setMat4("mv_matrix", modelViewMatrix);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    resourceManager->GetShaderProgram("DefaultShader")->setMat4("scale_matrix", trackGenerator->GetObstacleScaleMatrix());

    modelViewMatrix = camera->GetViewMatrix() * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -currentTime * Settings::OBSTACLES_SPEED_COEFF_ON_TRACK, 0.0f));

    if (currentTime * Settings::OBSTACLES_SPEED_COEFF_ON_TRACK >= 300 * (completedTracksNumber + 1))
    {
        track1 = track2;
        track2 = trackGenerator->GenerateTrackSection(300 * (completedTracksNumber + 2));
        completedTracksNumber++;
    }

    RenderTrackObstacles(track1, currentTime);
    RenderTrackObstacles(track2, currentTime);

    modelViewMatrix = camera->GetViewMatrix();
    modelViewMatrix *= glm::translate(glm::mat4(1.0f), player->GetPlayerPosition());
    resourceManager->GetShaderProgram("DefaultShader")->setMat4("mv_matrix", modelViewMatrix);

    glBindTexture(GL_TEXTURE_2D, resourceManager->GetTexture("Player")->getID());
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    auto collisionSubject = registrar->CheckIntersection(track1, player->GetPlayerPosition(), currentTime * Settings::OBSTACLES_SPEED_COEFF_ON_TRACK);

    if (collisionSubject != track1.end())
    {
        scoreInfo->ChangeScore(collisionSubject->score);
        scoreInfo->AddScoreInHistory(collisionSubject->score);

        track1.erase(collisionSubject);
    }

    modelViewMatrix = camera->GetViewMatrix();
    resourceManager->GetShaderProgram("TextShader")->use();
    resourceManager->GetShaderProgram("TextShader")->setMat4("proj_matrix", projectionMatrix);
    resourceManager->GetShaderProgram("TextShader")->setMat4("scale_matrix", glm::mat4(1.0f));
    resourceManager->GetShaderProgram("TextShader")->setMat4("mv_matrix", modelViewMatrix);

    RenderText("TextShader", "Arial", "Score: " + std::to_string(scoreInfo->getCurrentScore()), 250.0f, 270.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
    RenderScoreHistory(6, "TextShader", "Arial", 280.f, 240.0f, 0.3f);
}

void OpenGLManager::RecalculateProjectionMatrix(float aspect)
{
    projectionMatrix = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

Player* OpenGLManager::GetPlayer()
{
    return player;
}

void OpenGLManager::SetupVertices()
{
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    float square[18] =
    {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
    };

    float squareTextureCoords[12] =
    {
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareTextureCoords), squareTextureCoords, GL_STATIC_DRAW);

    //Text

    float textTextureCoords[12] =
    {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 3, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textTextureCoords), textTextureCoords, GL_STATIC_DRAW);
}

void OpenGLManager::RenderText(std::string shaderName, std::string fontName, std::string text, float x, float y, float scale, glm::vec3 color)
{
    resourceManager->GetShaderProgram(shaderName)->use();
    resourceManager->GetShaderProgram(shaderName)->setVec3("textColor", color);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    auto& Characters = resourceManager->GetFont(fontName)->GetCharacters();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    for (auto c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float verticesPositions[18] = 
        {
            xpos, ypos + h, 0.0f, xpos, ypos, 0.0f, xpos + w, ypos, 0.0f,
            xpos, ypos + h, 0.0f, xpos + w, ypos, 0.0f, xpos + w, ypos + h, 0.0f
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesPositions), verticesPositions);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLManager::RenderScoreHistory(unsigned int scoresNumber, std::string shaderName, std::string fontName, float x, float y, float scale)
{
    auto& scrHistory = scoreInfo->getReceivedScoresHistory();
    glm::vec3 color;
    std::string sign;

    float step = 20.0f;
    int i = 0;

    for (auto& iter : scrHistory)
    {
        if (iter < 0)
        {
            color = glm::vec3(1.0f, 0.0f, 0.0f);
            sign = "";
        }
        else
        {
            color = glm::vec3(0.0f, 1.0f, 0.0f);
            sign = "+";
        }

        RenderText(shaderName, fontName, sign + std::to_string(iter), x, y - step * i, scale, color);

        i++;
    }
}

void OpenGLManager::RenderTrackObstacles(std::list<Obstacle>& track, double currentTime)
{
    for (auto& obstacle : track)
    {
        if (CheckInTrackDomain(obstacle, currentTime))
        {
            resourceManager->GetShaderProgram("DefaultShader")->setMat4("mv_matrix", glm::translate(modelViewMatrix, obstacle.position));
            GLuint textureID = 0;

            if (obstacle.type == APPLE)
                textureID = resourceManager->GetTexture("Apple")->getID();
            else if (obstacle.type == CLOUD)
                textureID = resourceManager->GetTexture("Cloud")->getID();
            else if (obstacle.type == BANANA)
                textureID = resourceManager->GetTexture("Banana")->getID();
            else if (obstacle.type == ORANGE)
                textureID = resourceManager->GetTexture("Orange")->getID();
            else if (obstacle.type == BOMB)
                textureID = resourceManager->GetTexture("Bomb")->getID();

            glBindTexture(GL_TEXTURE_2D, textureID);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

bool OpenGLManager::CheckInTrackDomain(Obstacle& obs, double currentTime)
{
    // “”“ Õ¿ƒŒ —ƒ≈À¿“‹ ◊“Œ¡€ Œ¡À¿—“‹ ¡€À¿ ÕŒ–Ã¿À‹ÕŒ…, ¿ Õ≈  ŒÕ—“¿Õ“€

    if (obs.position.y - currentTime * Settings::OBSTACLES_SPEED_COEFF_ON_TRACK < 0 || obs.position.y - currentTime * Settings::OBSTACLES_SPEED_COEFF_ON_TRACK > 290)
        return false;

    return true;
}