#include "config.h"

Config& Config::instance() {
    static Config inst;
    return inst;
}

Config::Config() : settings("MyProject", "Murrikey") {}

void Config::save(const QString& key, const QVariant& val) {
    settings.setValue(key, val);
}

QVariant Config::load(const QString& key, const QVariant& def) {
    return settings.value(key, def);
}
