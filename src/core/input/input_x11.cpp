#include "input_x11.h"
#include "key_mapper/key_mapper.h"
#include "../utils/hardware_sync.h"
#include <cstring>
#include <X11/keysym.h>

InputX11Backend::InputX11Backend() {
    display = XOpenDisplay(nullptr);
    memset(oldKeys.data(), 0, oldKeys.size());
    memset(repeatCounters.data(), 0, repeatCounters.size());

    sysConfig = HardwareSync::getKeyboardSettings(display, 10);

    for (int i = 0; i < 256; ++i) {
        cachedKeySyms[i] = XkbKeycodeToKeysym(display, i, 0, 0);
    }
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

    for (int keycode = 8; keycode < 256; ++keycode) {
        bool isPressed = keys[keycode / 8] & (1 << (keycode % 8));
        bool wasPressed = oldKeys[keycode / 8] & (1 << (keycode % 8));

        if (!isPressed) {
            repeatCounters[keycode] = 0;
            continue;
        }

        KeySym sym = cachedKeySyms[keycode];
        if (KeyMapper::isModifier(sym)) continue;

        if (!wasPressed || shouldRepeat(keycode, isPressed)) {
            QString keyStr = mapKey(keycode, shift, caps);
            if (!keyStr.isEmpty() && onKey)
                onKey(keyStr, ctrl, shift, alt);
        }
    }

    memcpy(oldKeys.data(), keys, sizeof(keys));
}

bool InputX11Backend::shouldRepeat(int key, bool /*isPressed*/) {
    repeatCounters[key]++;
    if (repeatCounters[key] == sysConfig.delayThreshold)
        return true;
    if (repeatCounters[key] > sysConfig.delayThreshold)
        return (repeatCounters[key] - sysConfig.delayThreshold)
               % sysConfig.rateThreshold == 0;
    return false;
}

QString InputX11Backend::mapKey(int keycode, bool shift, bool caps) {
    KeySym sym;

    if (caps && XK_A <= cachedKeySyms[keycode] && cachedKeySyms[keycode] <= XK_Z) {
        sym = XkbKeycodeToKeysym(display, keycode, 0, 1);
    } else {
        sym = XkbKeycodeToKeysym(display, keycode, 0, shift ? 1 : 0);
    }

    return KeyMapper::map(sym, false);
}
