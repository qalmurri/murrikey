#include "key_mapper.h"
#include "config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <X11/Xlib.h>
#include <X11/keysym.h>

// Inisialisasi semua cache statis
QMap<unsigned long, QString> KeyMapper::symbolCache;
QMap<unsigned long, QString> KeyMapper::numlockCache; // Tambahkan ini agar tidak undefined

// Sesuaikan parameter di sini (tambah bool numLockOn)
QString KeyMapper::map(unsigned long sym, bool numLockOn) {
    if (symbolCache.isEmpty()) loadCache();
    
    // 1. Ambil nama asli dari X11
    char* name = XKeysymToString(sym);
    if (!name) return "";
    QString keyName(name);

    // 2. Cek Custom Mapping dari JSON (Prioritas Utama)
    QString jsonStr = Config::instance().load("custom_mapping", "{}").toString();
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonObject customMap = doc.object();
    
    if (customMap.contains(keyName)) {
        return customMap.value(keyName).toString();
    }

    // 3. LOGIKA NUMLOCK: Cek Numpad Cache jika NumLock aktif
    if (numLockOn && numlockCache.contains(sym)) {
        return numlockCache[sym];
    }
    
    // 4. Cek Simbol Standar
    if (symbolCache.contains(sym)) return symbolCache[sym];

    // 5. Cleanup Teknis X11
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
