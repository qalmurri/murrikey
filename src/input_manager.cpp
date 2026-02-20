#include "config.h"
#include <QDebug>
#include "input_manager.h"
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <cstring>
#undef Bool
#undef None
#undef Status
#undef Cursor
#undef KeyPress
#undef KeyRelease

void InputManager::setMode(int mode) {
    currentMode = mode;
}

QString InputManager::handleKeySym(unsigned long sym) {
    return mapToCustomDigit(sym); 
}

QString InputManager::handleKeyCode(int keycode) {
    return QString("KC_%1").arg(keycode);
}

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

QString InputManager::mapToCustomDigit(unsigned long sym) {
    static QMap<unsigned long, QString> digitMap = {
        {XK_a, "1"}, {XK_s, "2"}, {XK_d, "3"}, {XK_f, "4"},
        {XK_j, "5"}, {XK_k, "6"}, {XK_l, "7"}, {XK_semicolon, "8"}, // Perbaiki L menjadi XK_l
        {XK_space, "0"},
        {XK_Return, "\n"}
    };

    if (digitMap.contains(sym)) {
        return digitMap[sym];
    }

    char* name = XKeysymToString(sym);
    return name ? QString(name) : "";
}

void InputManager::check() {
    if (!display) return;

    char keys[32];
    XQueryKeymap(display, keys);
    XkbStateRec state;
    XkbGetState(display, XkbUseCoreKbd, &state);

    // 1. Definisikan modifier DULU agar bisa dipakai di bawah
    bool ctrl = state.mods & ControlMask;
    bool shift = state.mods & ShiftMask;
    bool alt = state.mods & Mod1Mask;
    bool caps = state.mods & LockMask;

    // 2. Loop utama untuk cek setiap bit keyboard
    for (int i = 0; i < 32; i++) {
        if (keys[i] != old_keys[i]) {
            for (int bit = 0; bit < 8; bit++) {
                // Jika tombol ditekan (bit berubah dari 0 ke 1)
                if ((keys[i] & (1 << bit)) && !(old_keys[i] & (1 << bit))) {
                    
                    int keycode = i * 8 + bit;
                    QString result;

                    // 3. Cek Mode (KeyCode vs KeySym)
                    if (currentMode == MODE_KEYCODE) {
                        result = handleKeyCode(keycode);
                    } else {
                        unsigned long sym = XkbKeycodeToKeysym(display, keycode, 0, (shift ^ caps) ? 1 : 0);
                        if (isModifier(sym)) continue;
                        result = handleKeySym(sym);
                    }

                    // 4. Kirim sinyal jika ada hasil
                    if (!result.isEmpty()) {
                        emit keyPressed(result, ctrl, shift, alt);
                        qDebug() << "Mode:" << currentMode << "Result:" << result;
                    }
                }
            }
        }
    }
    memcpy(old_keys, keys, 32);
}
