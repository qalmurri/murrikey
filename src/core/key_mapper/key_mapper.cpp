#include "key_mapper.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>

QString KeyMapper::map(unsigned long sym, bool numLockOn) {
    if (keyCache.isEmpty()) loadCache();

    if (keyCache.contains(sym)) {
        return numLockOn ? keyCache[sym].numlock : keyCache[sym].normal;
    }

    char* name = XKeysymToString(sym);
    if (!name) return "";

    QString keyName(name);

    if (specialCharMap.contains(keyName))
        return specialCharMap[keyName];

    return keyName;
}

bool KeyMapper::isModifier(unsigned long sym) {
    return modifiers.contains(sym);
}
