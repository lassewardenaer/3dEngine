#ifndef OSUTILS_H
#define OSUTILS_H

struct WindowSize {
    int width;
    int height;

    WindowSize(int width, int height) : width(width), height(height) {}
};

class OsUtils {
public:
    static WindowSize getSizeOfWindow();
};

#endif
