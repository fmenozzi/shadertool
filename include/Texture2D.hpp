#ifndef TEXTURE2D_HPP_
#define TEXTURE2D_HPP_

#include <glad/glad.h>

#include <stb_image.h>

#include <string>

class Texture2D {
private:
    GLuint         m_Texture;
    unsigned char* m_Image;
    int            m_Width;
    int            m_Height;

    // No copying or assignment
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

public:
    Texture2D()  { glGenTextures(1, &m_Texture); }
    ~Texture2D() { stbi_image_free(m_Image);     }

    Texture2D& loadFromFile(const std::string& filename);

    Texture2D& setWrapST(GLenum swrap, GLenum twrap);
    Texture2D& setMinMagFilter(GLenum min_filter, GLenum max_filter);
    Texture2D& setAnisotropicFiltering();

    Texture2D& generateTexture();

    Texture2D& bind(GLenum texture_unit);

    GLuint get() { return m_Texture; }
};

#endif
