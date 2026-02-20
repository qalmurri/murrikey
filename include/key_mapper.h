#ifndef KEY_MAPPER_H
#define KEY_MAPPER_H

#include <QString>

class KeyMapper {
public:
    // Gunakan unsigned long untuk sym agar cocok dengan X11
    static QString toCustomDigit(unsigned long sym);
    static bool isModifier(unsigned long sym);
};

#endif
