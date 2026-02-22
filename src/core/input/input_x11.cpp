#include "input_x11.h"
#include "../utils/hardware_sync.h"
#include "key_mapper/key_mapper.h"

#include <cstring>

InputX11Backend::InputX11Backend() {
    display = XOpenDisplay(nullptr);
    memset(old_keys, 0, sizeof(old_keys));
    memset(repeatTimers, 0, sizeof(repeatTimers));
    sysConfig = HardwareSync::getKeyboardSettings(display, 10);
}

InputX11Backend::~InputX11Backend() {
    if (display)
        XCloseDisplay(display);
}

void InputX11Backend::poll() {
    if (!display) return;

    char keys[32];
    XQueryKeymap(display, keys);

    XkbStateRec state;
    XkbGetState(display, XkbUseCoreKbd, &state);

    bool ctrl  = state.mods & ControlMask;
    bool shift = state.mods & ShiftMask;
    bool alt   = state.mods & Mod1Mask;
    bool caps  = state.mods & LockMask;

    for (int i = 8; i < 256; i++) {
        bool isPressed = keys[i / 8] & (1 << (i % 8));
        bool wasPressed = old_keys[i / 8] & (1 << (i % 8));

        if (!isPressed) {
            repeatTimers[i] = 0;
            continue;
        }

        KeySym sym = XkbKeycodeToKeysym(display, i, 0, 0);
        if (KeyMapper::isModifier(sym)) continue;

        if (!wasPressed || shouldRepeat(i)) {
            QString key = mapKey(i, sym, shift, caps);
            if (!key.isEmpty() && onKey) {
                onKey(key, ctrl, shift, alt);
            }
        }
    }

    memcpy(old_keys, keys, sizeof(old_keys));
}

bool InputX11Backend::shouldRepeat(int key) {
    repeatTimers[key]++;
    if (repeatTimers[key] == sysConfig.delayThreshold)
        return true;
    if (repeatTimers[key] > sysConfig.delayThreshold)
        return (repeatTimers[key] - sysConfig.delayThreshold)
               % sysConfig.rateThreshold == 0;
    return false;
}

QString InputX11Backend::mapKey(int keycode, KeySym sym, bool shift, bool caps) {
    unsigned long mapped =
        XkbKeycodeToKeysym(display, keycode, 0, (shift ^ caps) ? 1 : 0);
    return KeyMapper::map(mapped, false);
}
