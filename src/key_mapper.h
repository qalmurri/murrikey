#ifndef KEY_MAPPER_H
#define KEY_MAPPER_H
#include <QString>
class KeyMapper {
public:
    static QString toCustomDigit(unsigned long sym);
    static bool isModifier(unsigned long sym);
};
#endif
