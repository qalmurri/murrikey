#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <QVariant>

class Config {
public:
    static Config& instance();
    void save(const QString& key, const QVariant& val);
    QVariant load(const QString& key, const QVariant& def = QVariant());

private:
    Config();
    QSettings settings;
};

#endif
