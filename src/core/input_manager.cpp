#include "input_manager.h"
#include "key_mapper.h"
#include "config.h"
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <cstring>
#include <QDebug>

struct RecordContext {
    InputManager* manager;
    Display* ctrlDisplay;
};

InputManager::InputManager() {
    display = XOpenDisplay(NULL);
    memset(old_keys, 0, 32);
    currentMode = Config::instance().load("input_mode", MODE_KEYSYM).toInt();
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

    // 1. Ambil state modifier (Shift, Ctrl, Alt, Caps) dulu
    XkbStateRec state;
    XkbGetState(display, XkbUseCoreKbd, &state);
    bool ctrl = state.mods & ControlMask;
    bool shift = state.mods & ShiftMask;
    bool alt = state.mods & Mod1Mask;
    bool caps = state.mods & LockMask;

    // 2. Loop melalui 32 byte peta keyboard
    for (int i = 0; i < 32; i++) {
        // Optimasi: jika byte tidak berubah, skip
        if (keys[i] == old_keys[i]) continue;

        for (int bit = 0; bit < 8; bit++) {
            // Cek bit yang berubah dari 0 ke 1 (Key Press)
            if ((keys[i] & (1 << bit)) && !(old_keys[i] & (1 << bit))) {
                int keycode = i * 8 + bit;
                QString result;

                if (currentMode == MODE_KEYCODE) {
                    result = QString("KC_%1").arg(keycode);
                } else {
                    // Gunakan XkbKeycodeToKeysym untuk menangani Shift & CapsLock
                    unsigned long sym = XkbKeycodeToKeysym(display, keycode, 0, (shift ^ caps) ? 1 : 0);
                    
                    // Gunakan KeyMapper yang baru kita buat
                    if (KeyMapper::isModifier(sym)) continue;
                    result = KeyMapper::map(sym); 
                }

                if (!result.isEmpty()) {
                    emit keyPressed(result, ctrl, shift, alt);
                }
            }
        }
    }
    memcpy(old_keys, keys, 32);
}
