#pragma once
#include <Key.hpp>
#include <string>

using namespace std;

struct Scroll { double dx, dy; };
struct MousePos { double x, y; };
struct WindowRect { int width, height; };

namespace System {
    namespace Input {
        bool isKeyPressed(Key key);
        Scroll getScroll();
        MousePos getMousePos();
    }

    namespace Graphics {
        WindowRect getWindowRect();
        float getAspectRatio();
    }

    void close();
}
