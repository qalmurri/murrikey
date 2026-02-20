#include "input_manager.h"
#include "key_mapper.h"
#include "config.h"
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <cstring>
#include <QDebug>
#undef Bool
#undef None
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
bool InputManager::isModifier(unsigned long s) {
    return (s == XK_Control_L || s == XK_Control_R || s == XK_Shift_L || 
            s == XK_Shift_R || s == XK_Alt_L || s == XK_Alt_R || s == XK_Super_L);
}
QString InputManager::mapToCustomDigit(unsigned long sym) {
    static QMap<unsigned long, QString> digitMap = {
        {XK_a, "1"}, {XK_s, "2"}, {XK_d, "3"}, {XK_f, "4"},
        {XK_j, "5"}, {XK_k, "6"}, {XK_l, "7"}, {XK_semicolon, "8"},
        {XK_space, "0"}, {XK_Return, "\n"}
    };
    if (digitMap.contains(sym)) return digitMap[sym];
    char* name = XKeysymToString(sym);
    return name ? QString(name) : "";
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
                    QString result;
                    if (currentMode == MODE_KEYCODE) {
                        result = QString("KC_%1").arg(keycode);
                    } else {
                        unsigned long sym = XkbKeycodeToKeysym(display, keycode, 0, (shift ^ caps) ? 1 : 0);
                        if (KeyMapper::isModifier(sym)) continue; // Gunakan KeyMapper
                        result = KeyMapper::toCustomDigit(sym);   // Gunakan KeyMapper
                    }
                    if (!result.isEmpty()) emit keyPressed(result, ctrl, shift, alt);
                }
            }
        }
    }
    memcpy(old_keys, keys, 32);
}
