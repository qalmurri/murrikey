#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

#include "key_mapper.h"
#include "config.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>

#undef Bool
#undef Cursor
#undef None
#undef Status

QString KeyMapper::toCustomDigit(unsigned long sym) {
    QString jsonStr = Config::instance().load("custom_mapping", "{}").toString();
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonObject mapping = doc.object();

    char* name = XKeysymToString(sym);
    if (!name) return "";
    
    QString keyName(name);

    if (mapping.contains(keyName)) {
        return mapping.value(keyName).toString();
    }

    return keyName;
}

bool KeyMapper::isModifier(unsigned long s) {
    return (s == XK_Control_L || s == XK_Control_R || s == XK_Shift_L || 
            s == XK_Shift_R || s == XK_Alt_L || s == XK_Alt_R || s == XK_Super_L);
}
