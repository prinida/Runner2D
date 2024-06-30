#include "ResourcesManager.h"
#include "ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ResourceManager::ResourceManager(const std::string& executablePath)
{
    size_t found = executablePath.find_last_of("/\\");
    path = executablePath.substr(0, found);
}

std::string ResourceManager::GetFileString(const std::string& relativeFilePath) const
{
    std::ifstream f;
    f.open(path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);

    if (!f.is_open())
    {
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        return std::string();
    }

    std::stringstream buffer;
    buffer << f.rdbuf();

    return buffer.str();
}

std::shared_ptr<ShaderProgram> ResourceManager::LoadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vertexString = GetFileString(vertexPath);

    if (vertexString.empty())
    {
        std::cerr << "No vertex shader!" << std::endl;
        return nullptr;
    }

    std::string fragmentString = GetFileString(fragmentPath);

    if (fragmentString.empty())
    {
        std::cerr << "No fragment shader!" << std::endl;
        return nullptr;
    }

    auto& newShader = shaderPrograms.emplace(shaderName, std::make_shared<ShaderProgram>(vertexString, fragmentString)).first->second;

    if (newShader->getIsCompiled())
        return newShader;

    std::cerr << "Can't load shader program:\n"
        << "Vertex: " << vertexPath << "\n"
        << "Fragment:" << fragmentPath << std::endl;

    return nullptr;
}

std::shared_ptr<ShaderProgram> ResourceManager::GetShaderProgram(const std::string& shaderName)
{
    ShaderProgramsMap::const_iterator it = shaderPrograms.find(shaderName);

    if (it != shaderPrograms.end())
        return it->second;

    std::cerr << "Can't find the shader program: " << shaderName << std::endl;

    return nullptr;
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string& textureName, const std::string& texturePath)
{
    int nrChannels = 0, width = 0, height = 0;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* pixels = stbi_load(std::string(path + "/" + texturePath).c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

    if (!pixels)
    {
        std::cerr << "Can't load image: " << texturePath << std::endl;

        stbi_image_free(pixels);

        return nullptr;
    }
    else
    {
        auto& newTexture = textures.emplace(textureName, std::make_shared<Texture>(pixels, width, height)).first->second;

        stbi_image_free(pixels);

        return newTexture;
    }
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& textureName)
{
    TexturesMap::const_iterator it = textures.find(textureName);

    if (it != textures.end())
        return it->second;

    std::cerr << "Can't find the texture: " << textureName << std::endl;

    return nullptr;
}

std::shared_ptr<Font> ResourceManager::LoadFont(const std::string& fontName, const std::string& fontPath)
{
    auto& newFont = fonts.emplace(fontName, std::make_shared<Font>(std::string(path + "/" + fontPath))).first->second;

    return newFont;
}

std::shared_ptr<Font> ResourceManager::GetFont(const std::string& fontName)
{
    FontsMap::const_iterator it = fonts.find(fontName);

    if (it != fonts.end())
        return it->second;

    std::cerr << "Can't find the font: " << fontName << std::endl;

    return nullptr;
}
