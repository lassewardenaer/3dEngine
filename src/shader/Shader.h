#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    Shader();
    ~Shader();
    
    bool LoadFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    bool LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    
    void Use() const;
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetMat4(const std::string& name, const glm::mat4& value) const;
    
    unsigned int GetID() const { return m_id; }
    
private:
    unsigned int m_id;
    
    bool CompileShader(unsigned int shader, const std::string& source);
    bool LinkProgram(unsigned int vertex, unsigned int fragment);
    std::string ReadFile(const std::string& path);
};

#endif

