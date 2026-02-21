#include "input_manager.h"
#include "key_mapper.h"
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

void InputManager::processKeyChange(const char* keys, bool numLockOn, bool ctrl, bool shift, bool alt, bool caps) {
    for (int i = 0; i < 32; i++) {
        if (keys[i] == old_keys[i]) continue;

        for (int bit = 0; bit < 8; bit++) {
            if ((keys[i] & (1 << bit)) && !(old_keys[i] & (1 << bit))) {
                int keycode = i * 8 + bit;
                QString result;

                if (currentMode == 1) { // MODE_KEYCODE
                    result = QString("KC_%1").arg(keycode);
                } else {
                    unsigned long sym = XkbKeycodeToKeysym(display, keycode, 0, (shift ^ caps) ? 1 : 0);
                    if (KeyMapper::isModifier(sym)) continue;
                    result = KeyMapper::map(sym, numLockOn);
                }

                if (!result.isEmpty()) {
                    emit keyPressed(result, ctrl, shift, alt);
                }
            }
        }
    }
}
