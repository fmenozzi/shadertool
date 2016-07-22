#define STB_IMAGE_IMPLEMENTATION
#include <Texture2D.hpp>

#include <GLFW/glfw3.h>

Texture2D& Texture2D::loadFromFile(const std::string& filename) {
    auto path = std::string(PROJECT_ROOT "/resources/") + filename;

    m_Image = stbi_load(path.c_str(), &m_Width, &m_Height, 0, 3);
    if (!m_Image)
        fprintf(stderr, "Failed to load image %s\n", path.c_str());

    return *this;
}

Texture2D& Texture2D::setWrapST(GLenum swrap, GLenum twrap) {
    // TODO: Validate swrap and twrap
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, swrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, twrap);

    return *this;
}

Texture2D& Texture2D::setMinMagFilter(GLenum min_filter, GLenum mag_filter) {
    // TODO: Validate min_filter and mag_filter
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

    return *this;
}

Texture2D& Texture2D::setAnisotropicFiltering() {
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    if (glfwExtensionSupported("GL_EXT_texture_filter_anisotropic")) {
        GLfloat anisotropy = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
    } else {
        fprintf(stderr, "Anisotropic filtering extension unavailable\n");
    }

    return *this;
}

Texture2D& Texture2D::generateTexture() {
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Image);
    glGenerateMipmap(GL_TEXTURE_2D);

    return *this;
}

Texture2D& Texture2D::bind(GLenum texture_unit) {
    // TODO: Validate texture_unit
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    return *this;
}
