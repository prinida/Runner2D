#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H

#include "ShaderProgram.h"
#include "Texture.h"
#include "Font.h"

#include <string>
#include <memory>
#include <map>

class ResourceManager
{
public:
    ResourceManager(const std::string& executablePath);
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;
    ResourceManager(ResourceManager&&) = delete;

    std::shared_ptr<ShaderProgram> LoadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
    std::shared_ptr<ShaderProgram> GetShaderProgram(const std::string& shaderName);

    std::shared_ptr<Texture> LoadTexture(const std::string& textureName, const std::string& texturePath);
    std::shared_ptr<Texture> GetTexture(const std::string& textureName);

    std::shared_ptr<Font> LoadFont(const std::string& fontName, const std::string& fontPath);
    std::shared_ptr<Font> GetFont(const std::string& fontName);

private:
    std::string GetFileString(const std::string& relativeFilePath) const;

    typedef std::map<const std::string, std::shared_ptr<ShaderProgram>> ShaderProgramsMap;
    ShaderProgramsMap shaderPrograms;

    typedef std::map<const std::string, std::shared_ptr<Texture>> TexturesMap;
    TexturesMap textures;

    typedef std::map<const std::string, std::shared_ptr<Font>> FontsMap;
    FontsMap fonts;

    std::string path;
};

#endif
