#include "OsUtils.h"

#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>
#elif defined(_WIN32)
#include <windows.h>
#else
#include <X11/Xlib.h>
#endif

WindowSize OsUtils::getSizeOfWindow() {
    CGDirectDisplayID displayID = CGMainDisplayID();
    size_t width = CGDisplayPixelsWide(displayID);
    size_t height = CGDisplayPixelsHigh(displayID);
    return WindowSize(static_cast<int>(width), static_cast<int>(height));
}
