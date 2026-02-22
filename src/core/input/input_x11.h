#ifndef INPUT_X11_H
#define INPUT_X11_H

#include "input_backend.h"
#include "../utils/hardware_sync.h"

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <QElapsedTimer>
#include <array>

class InputX11Backend : public InputBackend {
public:
    InputX11Backend();
    ~InputX11Backend() override;

    void poll() override;

private:
    bool shouldRepeat(int key, bool isPressed);
    QString mapKey(int keycode, bool shift, bool caps);

    Display* display = nullptr;
    std::array<char, 32> oldKeys{};
    std::array<int, 256> repeatCounters{};
    std::array<QElapsedTimer, 256> lastPressTime{};
    std::array<KeySym, 256> cachedKeySyms{};
    KeyboardConfig sysConfig;
};

#endif
