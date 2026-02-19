#include "preferences.h"
#include "config.h"
#include <QFormLayout>
#include <QSlider>
#include <QPushButton>
#include <QColorDialog>
#include <QFontDialog>

PreferencesWindow::PreferencesWindow() {
    setWindowTitle("Murrikey Settings");
    auto* layout = new QFormLayout(this);

    auto* ySlider = new QSlider(Qt::Horizontal);
    ySlider->setRange(0, 1080);
    ySlider->setValue(Config::instance().load("y", 800).toInt());

    auto* colorBtn = new QPushButton("Pick Color");
    auto* fontBtn = new QPushButton("Pick Font");

    layout->addRow("Vertical Position:", ySlider);
    layout->addRow("Color:", colorBtn);
    layout->addRow("Font:", fontBtn);

    connect(ySlider, &QSlider::valueChanged, this, [this](int v) {
        Config::instance().save("y", v); emit configChanged();
    });

    connect(colorBtn, &QPushButton::clicked, this, [this]() {
        QColor c = QColorDialog::getColor(Qt::green, this);
        if (c.isValid()) { Config::instance().save("color", c.name()); emit configChanged(); }
    });

    connect(fontBtn, &QPushButton::clicked, this, [this]() {
        bool ok; QFont f = QFontDialog::getFont(&ok, QFont("Monospace", 36), this);
        if (ok) { Config::instance().save("font_family", f.family()); emit configChanged(); }
    });
}
