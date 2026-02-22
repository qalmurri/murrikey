#ifndef KEY_MAPPER_H
#define KEY_MAPPER_H

#include <QString>
#include <QMap>
#include <QSet>
#include <QPair>

class KeyMapper {
public:
    static QString map(unsigned long sym, bool numLockOn);
    static bool isModifier(unsigned long sym);
    static void loadCache();

private:
    struct KeyEntry {
        QString normal;
        QString numlock;
    };

    static QMap<unsigned long, KeyEntry> keyCache;
    static QMap<QString, QString> specialCharMap;
    static QSet<unsigned long> modifiers;
};

#endif
