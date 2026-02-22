#ifndef KEY_MAPPER_H
#define KEY_MAPPER_H

#include <QString>
#include <QMap>

class KeyMapper {
public:
    static QString map(unsigned long sym, bool numLockOn);
    static bool isModifier(unsigned long sym);
    static void loadCache();

private:
    static QMap<unsigned long, QString> symbolCache;
    static QMap<unsigned long, QString> numlockCache;
};

#endif
