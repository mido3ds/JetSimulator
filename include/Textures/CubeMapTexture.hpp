#pragma once
#include "Texture.hpp"
#include <vector>
#include <string>

using namespace std;

class CubeMapTexture: public Texture {
private:
    vector<string> paths;
public:
    /**
     * dir: folder of cube texture
     * rest: files names of cube sides
     **/
    CubeMapTexture(const string& dir, 
        const string& right,
        const string& left,
        const string& top,
        const string& bottom,
        const string& back,
        const string& front);

    virtual void load() override;
};