#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#define GLSL(src) "#version 150 core\n" #src

class Shader {
private:
    GLuint m_Program;

    // No copying or assignment
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void bind(GLint location, GLfloat value);
    void bind(GLint location, GLint value);
    void bind(GLint location, const glm::vec2& vec);
    void bind(GLint location, const glm::vec3& vec);
    void bind(GLint location, const glm::vec4& vec);
    void bind(GLint location, const glm::mat4& mat);

public:
    Shader()  { m_Program = glCreateProgram(); }
    ~Shader() { glDeleteProgram(m_Program);    }

    Shader& attachFromFile(const std::string& filename);
    Shader& attachFromText(GLenum type, const std::string& text);

    template<class T>
    Shader& bind(T&& value, const std::string& name) {
        int location = glGetUniformLocation(m_Program, name.c_str());
        if (location != -1) {
            bind(location, std::forward<T>(value));
        }
        return *this;
    }

    Shader& link();

    Shader& use();

    GLuint get() { return m_Program; }
};

#endif
