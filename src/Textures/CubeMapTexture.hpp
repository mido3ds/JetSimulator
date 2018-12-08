#pragma once
#include "Texture.hpp"
#include <vector>
#include <string>

class CubeMapTexture: public Texture {
private:
    std::vector<std::string> paths;
public:
    /**
     * dir: folder of cube texture
     * rest: files names of cube sides
     **/
    CubeMapTexture(const std::string& dir, 
        const std::string& right,
        const std::string& left,
        const std::string& top,
        const std::string& bottom,
        const std::string& back,
        const std::string& front);

    virtual void load() override;
};