namespace System {
    void _init();
    void _mainLoop();
    void _destroy();
}

int main() {
    System::_init();
    System::_mainLoop();
    System::_destroy();

    return 0;
}
