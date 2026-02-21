#include <QObject>
#include <QString>
#include "input_manager.h"           // Header ini isinya Qt
#include "../utils/hardware_sync.h"  // Header ini sudah kita bersihkan dari X11
#include "config.h"
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>
#include <cstring>

InputManager::InputManager() {
    display = XOpenDisplay(NULL);
    memset(old_keys, 0, 32);
    memset(repeatTimers, 0, sizeof(repeatTimers));

    sysConfig = HardwareSync::getKeyboardSettings(display, 10);

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
    
    XkbStateRec state;
    XkbGetState(display, XkbUseCoreKbd, &state);
    
    bool ctrl  = state.mods & ControlMask;
    bool shift = state.mods & ShiftMask;
    bool alt   = state.mods & Mod1Mask;
    bool caps  = state.mods & LockMask;

    for (int i = 8; i < 256; i++) {
        bool isPressed = keys[i / 8] & (1 << (i % 8));
        bool wasPressed = old_keys[i / 8] & (1 << (i % 8));
    
        if (isPressed) {
            // Ambil KeySym untuk cek apakah ini modifier
            KeySym sym = XkbKeycodeToKeysym(display, i, 0, 0);
            bool isModifier = (sym >= XK_Shift_L && sym <= XK_Hyper_R);
    
            if (!wasPressed) {
                // JANGAN KIRIM jika hanya tombol modifier yang ditekan sendirian
                if (!isModifier) {
                    sendKey(i, ctrl, shift, alt, caps);
                }
                repeatTimers[i] = 0;
            } else {
                // LOGIKA REPEAT: Hanya untuk tombol NON-MODIFIER
                if (!isModifier) {
                    repeatTimers[i]++;
                    if (repeatTimers[i] == sysConfig.delayThreshold) {
                        sendKey(i, ctrl, shift, alt, caps);
                    } else if (repeatTimers[i] > sysConfig.delayThreshold) {
                        if ((repeatTimers[i] - sysConfig.delayThreshold) % sysConfig.rateThreshold == 0) {
                            sendKey(i, ctrl, shift, alt, caps);
                        }
                    }
                }
            }
        } else {
            repeatTimers[i] = 0;
        }
    }
    memcpy(old_keys, keys, 32);
}

void InputManager::sendKey(int keycode, bool ctrl, bool shift, bool alt, bool caps) {
    // 1. Siapkan XKeyEvent tiruan agar XLookupString bisa bekerja
    XKeyEvent event;
    event.display = display;
    event.keycode = keycode;
    event.state   = 0;
    if (shift) event.state |= ShiftMask;
    if (caps)  event.state |= LockMask;
    // Kita sengaja tidak masukkan Ctrl/Alt di state agar XLookupString 
    // tetap mengembalikan karakter aslinya (misal 'c' bukan control-char)

    char buffer[32];
    KeySym sym;
    // 2. Terjemahkan keycode menjadi string asli
    int len = XLookupString(&event, buffer, sizeof(buffer), &sym, NULL);

    QString keyName;
    if (len > 0) {
        buffer[len] = '\0';
        keyName = QString::fromUtf8(buffer);
    } else {
        // Jika bukan karakter (seperti F1, Home, dsb), ambil nama Keysym-nya
        const char* s = XKeysymToString(sym);
        if (s) keyName = QString::fromUtf8(s);
    }

    if (keyName.isEmpty()) return;

    // 3. Normalisasi Nama Tombol
    if (keyName == "\r" || keyName == "Return") keyName = "Enter";
    else if (keyName == "\b" || keyName == "BackSpace") keyName = "⌫";
    else if (keyName == "\t") keyName = "Tab";
    else if (keyName == "Escape") keyName = "Esc";
    else if (keyName == "space" || keyName == " ") keyName = " "; // Pastikan space bersih

    // 4. Kirim ke UI
    emit keyPressed(keyName, ctrl, shift, alt);
}
