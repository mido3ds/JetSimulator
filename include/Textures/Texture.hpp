#pragma once
#include <glad/glad.h>

class Texture {
protected:
    GLuint texId = 0, type, unit;
public:
    /**
     * type: Must be one of GL_TEXTURE_1D, GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D, 
     *  GL_TEXTURE_2D_ARRAY, GL_TEXTURE_2D_MULTISAMPLE, 
     *  GL_TEXTURE_2D_MULTISAMPLE_ARRAY, GL_TEXTURE_3D, 
     *  GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY, or GL_TEXTURE_RECTANGLE.
     * 
     * unit: 0, 1, 2, ...
     * 
     * path: where it's located
     **/
    Texture(GLint type, GLuint unit);
    virtual ~Texture();

    virtual void load()=0;
    bool isLoaded() const;
    void bind() const;
    void setParameter(GLenum pname, GLint param);
    GLint getParameter(GLenum pname);
};
