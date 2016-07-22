#include <Shader.hpp>

#include <string>
#include <fstream>
#include <memory>

template<class T> using iter = std::istreambuf_iterator<T>;

void Shader::bind(GLint location, GLfloat value) {
    glUniform1f(location, value);
}

void Shader::bind(GLint location, GLint value) {
    glUniform1i(location, value);
}

void Shader::bind(GLint location, const glm::vec3& vec) {
    glUniform3fv(location, 1, glm::value_ptr(vec));
}

void Shader::bind(GLint location, const glm::vec4& vec) {
    glUniform4fv(location, 1, glm::value_ptr(vec));
}

void Shader::bind(GLint location, const glm::mat4& mat) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

Shader& Shader::attachFromFile(const std::string& filename) {
    GLenum type = GL_INVALID_ENUM;
    auto dot = filename.rfind("."); 
    auto ext = filename.substr(dot+1);
         if (ext == "vert") type = GL_VERTEX_SHADER;
    else if (ext == "frag") type = GL_FRAGMENT_SHADER;
    else if (ext == "geom") type = GL_GEOMETRY_SHADER;
    else if (ext == "comp") type = GL_COMPUTE_SHADER;

    if (type == GL_INVALID_ENUM) {
        fprintf(stderr, "Invalid shader file extension for %s\n", filename.c_str());
        return *this;
    }

    auto fullpath = std::string(PROJECT_ROOT "/shaders/") + filename;

    std::ifstream fd(fullpath);

    if (!fd.is_open()) {
        fprintf(stderr, "File %s not found\n", fullpath.c_str());
        return *this;
    }

    return attachFromText(type, std::string(iter<char>(fd), iter<char>()));    
}

Shader& Shader::attachFromText(GLenum type, const std::string& text) {
    GLuint shader = glCreateShader(type);

    GLint status;
    GLint length;

    const char* src = text.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        auto buf = std::unique_ptr<char[]>(new char[length]);
        glGetShaderInfoLog(shader, length, nullptr, buf.get());
        fprintf(stderr, "Failed to compile shader\n");
        fprintf(stderr, "%s\n", buf.get());
    }

    glAttachShader(m_Program, shader);
    glDeleteShader(shader);

    return *this;
}

Shader& Shader::link() {
    GLint status;
    GLint length;

    glLinkProgram(m_Program);
    glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &length);
        auto buf = std::unique_ptr<char[]>(new char[length]);
        glGetProgramInfoLog(m_Program, length, nullptr, buf.get());
        fprintf(stderr, "Failed to link shader program\n");
        fprintf(stderr, "%s\n", buf.get());
    }

    return *this;
}

Shader& Shader::use() {
    glUseProgram(m_Program);
    return *this;
}
