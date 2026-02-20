#include "key_mapper.h"
#include "config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <X11/Xlib.h>
#include <X11/keysym.h>

// Inisialisasi cache statis
QMap<unsigned long, QString> KeyMapper::symbolCache;

void KeyMapper::loadCache() {
    symbolCache[XK_space] = " ";
    symbolCache[XK_Return] = "⏎";
    symbolCache[XK_BackSpace] = "⌫";
    symbolCache[XK_Tab] = "⇥";
    symbolCache[XK_Escape] = "⎋";
    symbolCache[XK_Left] = "←";
    symbolCache[XK_Right] = "→";
    symbolCache[XK_Up] = "↑";
    symbolCache[XK_Down] = "↓";
}

QString KeyMapper::map(unsigned long sym) {
    if (symbolCache.isEmpty()) loadCache();
    
    // 1. Ambil nama asli dari X11 dulu
    char* name = XKeysymToString(sym);
    if (!name) return "";
    QString keyName(name);

    // 2. Cek Custom Mapping dari Config (JSON) - Biar fitur lama tetep jalan
    QString jsonStr = Config::instance().load("custom_mapping", "{}").toString();
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonObject customMap = doc.object();
    
    if (customMap.contains(keyName)) {
        return customMap.value(keyName).toString();
    }

    // 3. Cek Simbol Cantik (⏎, ⌫, dll)
    if (symbolCache.contains(sym)) return symbolCache[sym];

    // 4. Cleanup nama teknis X11
    if (keyName == "bracketleft") return "[";
    if (keyName == "bracketright") return "]";
    if (keyName == "semicolon") return ";";
    if (keyName == "apostrophe") return "'";
    
    return keyName;
}

bool KeyMapper::isModifier(unsigned long s) {
    return (s == XK_Control_L || s == XK_Control_R || s == XK_Shift_L || 
            s == XK_Shift_R || s == XK_Alt_L || s == XK_Alt_R || s == XK_Super_L);
}
