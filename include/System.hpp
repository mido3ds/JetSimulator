#pragma once
#include <Key.hpp>
#include <string>

using namespace std;

struct Scroll { double dx, dy; };
struct MousePos { double x, y; };

namespace System {
    namespace Input {
        bool isKeyPressed(Key key);
        Scroll getScroll();
        MousePos getMousePos();
    }

    namespace Graphics {
        int getWidth();
        int getHeight();
        float getAspectRatio();
    }

    void close();
}
