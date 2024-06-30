#include "ShaderProgram.h"

#include <iostream>

ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint vertexShaderID, fragmentShaderID;

    if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
    {
        std::cerr << "VERTEX::SHADER: Compile time error:\n" << std::endl;
        return;
    }

    if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
    {
        std::cerr << "FRAGMENT::SHADER: Compile time error:\n" << std::endl;
        glDeleteShader(vertexShaderID);
        return;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexShaderID);
    glAttachShader(ID, fragmentShaderID);
    glLinkProgram(ID);

    GLint success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if (!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(ID, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER: Link time error:\n" << infoLog << std::endl;
    }
    else
        isCompiled = true;

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
{
    shaderID = glCreateShader(shaderType);
    const char* code = source.c_str();
    glShaderSource(shaderID, 1, &code, nullptr);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER: Compile time error:\n" << infoLog << std::endl;
        return false;
    }

    return true;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(ID);
}

bool ShaderProgram::getIsCompiled() const
{
    return this->isCompiled;
}

void ShaderProgram::use() const
{
    glUseProgram(ID);
}

GLuint ShaderProgram::getID()
{
    return this->ID;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
{
    glDeleteProgram(ID);
    ID = shaderProgram.ID;
    isCompiled = shaderProgram.isCompiled;

    shaderProgram.ID = 0;
    shaderProgram.isCompiled = false;

    return *this;
}

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
{
    ID = shaderProgram.ID;
    isCompiled = shaderProgram.isCompiled;

    shaderProgram.ID = 0;
    shaderProgram.isCompiled = false;
}

// utility uniform functions
 // ------------------------------------------------------------------------
void ShaderProgram::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void ShaderProgram::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void ShaderProgram::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}