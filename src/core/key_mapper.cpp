#include "key_mapper.h"
#include "config.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>

QMap<unsigned long, QString> KeyMapper::symbolCache;
QMap<unsigned long, QString> KeyMapper::numlockCache; 

QString KeyMapper::map(unsigned long sym, bool numLockOn) {
    if (symbolCache.isEmpty()) loadCache();
    
    char* name = XKeysymToString(sym);
    if (!name) return "";
    QString keyName(name);

    // --- FITUR CUSTOM MAPPING DIHAPUS ---

    if (numLockOn && numlockCache.contains(sym)) {
        return numlockCache[sym];
    }
    
    if (symbolCache.contains(sym)) return symbolCache[sym];

    // Normalisasi karakter standar X11
    if (keyName == "bracketleft") return "[";
    if (keyName == "bracketright") return "]";
    if (keyName == "semicolon") return ";";
    if (keyName == "apostrophe") return "'";
    if (keyName == "comma") return ",";
    if (keyName == "period") return ".";
    if (keyName == "slash") return "/";
    if (keyName == "backslash") return "\\";
    if (keyName == "minus") return "-";
    if (keyName == "equal") return "=";
    if (keyName == "grave") return "`";
    
    return keyName;
}

bool KeyMapper::isModifier(unsigned long s) {
    return (s == XK_Control_L || s == XK_Control_R || s == XK_Shift_L || 
            s == XK_Shift_R || s == XK_Alt_L || s == XK_Alt_R || s == XK_Super_L);
}
