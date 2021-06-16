#include <Textures/CubeMapTexture.hpp>
#include <Textures/Image.hpp>"

CubeMapTexture::CubeMapTexture(const string& dir, 
        const string& right,
        const string& left,
        const string& top,
        const string& bottom,
        const string& back,
        const string& front) 
    :Texture(GL_TEXTURE_CUBE_MAP, 0) {
    paths.push_back(dir+left);
    paths.push_back(dir+right);
    paths.push_back(dir+bottom);
    paths.push_back(dir+top);
    paths.push_back(dir+back);
    paths.push_back(dir+front);
}

void CubeMapTexture::load() {
    glGenTextures(1, &texId);
    bind();

    int width, height, nrChannels;
    unsigned char* data;

    for (int i = 0; i < paths.size(); i++) {
        Image image(paths[i]);
        image.load();

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, image.glInternalFormat, 
            image.width, image.height, 0, image.glFormat, GL_UNSIGNED_BYTE, image.data);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}