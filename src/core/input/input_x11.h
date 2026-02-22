#ifndef INPUT_X11_H
#define INPUT_X11_H

#include "input_backend.h"
#include "../utils/hardware_sync.h"

#include <X11/Xlib.h>
#include <X11/XKBlib.h>

class InputX11Backend : public InputBackend {
public:
    InputX11Backend();
    ~InputX11Backend() override;

    void poll() override;

private:
    bool shouldRepeat(int key);
    QString mapKey(int keycode, KeySym sym, bool shift, bool caps);

    Display* display = nullptr;
    char old_keys[32]{};
    int repeatTimers[256]{};
    KeyboardConfig sysConfig;
};

#endif
