#include <QDebug> // Include Qt dulu
#include "input_manager.h"

// Baru masukkan X11
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <cstring>

// MATIKAN MACRO X11 YANG MERUSAK QT
#undef Bool
#undef None
#undef Status
#undef Cursor
#undef KeyPress
#undef KeyRelease

InputManager::InputManager() {
    display = XOpenDisplay(NULL);
    memset(old_keys, 0, 32);
}

InputManager::~InputManager() {
    if (display) XCloseDisplay(display);
}

bool InputManager::isModifier(unsigned long s) {
    return (s == XK_Control_L || s == XK_Control_R || s == XK_Shift_L || 
            s == XK_Shift_R || s == XK_Alt_L || s == XK_Alt_R || s == XK_Super_L);
}

void InputManager::check() {
    if (!display) return;

    char keys[32];
    XQueryKeymap(display, keys);
    XkbStateRec state;
    XkbGetState(display, XkbUseCoreKbd, &state);

    bool ctrl = state.mods & ControlMask;
    bool shift = state.mods & ShiftMask;
    bool alt = state.mods & Mod1Mask;
    bool caps = state.mods & LockMask;

    for (int i = 0; i < 32; i++) {
        if (keys[i] != old_keys[i]) {
            for (int bit = 0; bit < 8; bit++) {
                if ((keys[i] & (1 << bit)) && !(old_keys[i] & (1 << bit))) {
                    int keycode = i * 8 + bit;
                    // XkbKeycodeToKeysym butuh Display*
                    unsigned long sym = XkbKeycodeToKeysym(display, keycode, 0, (shift ^ caps) ? 1 : 0);
                    
                    if (isModifier(sym)) continue;
                    
                    char* name = XKeysymToString(sym);
                    if (name) emit keyPressed(QString(name), ctrl, shift, alt);
                }
            }
        }
    }
    memcpy(old_keys, keys, 32);
}
