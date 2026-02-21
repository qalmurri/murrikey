#include "input_manager.h"
#include "config.h"
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <cstring>

InputManager::InputManager() {
    display = XOpenDisplay(NULL);
    memset(old_keys, 0, 32);
    currentMode = Config::instance().load("input_mode", 0).toInt();
}

InputManager::~InputManager() {
    if (display) XCloseDisplay(display);
}

void InputManager::setMode(int mode) {
    currentMode = mode;
}

void InputManager::check() {
    if (!display) return;

    char keys[32];
    XQueryKeymap(display, keys);

    // Ambil Modifier State
    XkbStateRec state;
    XkbGetState(display, XkbUseCoreKbd, &state);
    
    bool numLockOn = state.mods & Mod2Mask;
    bool ctrl      = state.mods & ControlMask;
    bool shift     = state.mods & ShiftMask;
    bool alt       = state.mods & Mod1Mask;
    bool caps      = state.mods & LockMask;

    // Serahkan proses deteksi bit ke file sebelah
    processKeyChange(keys, numLockOn, ctrl, shift, alt, caps);
    
    memcpy(old_keys, keys, 32);
}
