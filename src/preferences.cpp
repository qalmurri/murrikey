// src/preferences.cpp
#include "preferences.h"
#include "config.h"
#include <QFormLayout>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QFontDialog>
#include <QComboBox>

PreferencesWindow::PreferencesWindow() {
    setWindowTitle("Murrikey Settings");
    auto* layout = new QFormLayout(this);

    // Slider Posisi Vertikal
    auto* ySlider = new QSlider(Qt::Horizontal);
    ySlider->setRange(0, 1080);
    ySlider->setValue(Config::instance().load("y", 800).toInt());

    // Slider Durasi Muncul (Hide Duration)
    auto* durationSlider = new QSlider(Qt::Horizontal);
    durationSlider->setRange(500, 10000); // 0.5 detik sampai 10 detik
    durationSlider->setValue(Config::instance().load("hide_duration", 3000).toInt());
    auto* durationLabel = new QLabel(QString("%1 ms").arg(durationSlider->value()));

    auto* colorBtn = new QPushButton("Pick Color");
    auto* fontBtn = new QPushButton("Pick Font");

    auto* modeCombo = new QComboBox();
    modeCombo->addItem("Character Mode (KeySym)", 0);
    modeCombo->addItem("Hardware Mode (KeyCode)", 1);

    int savedMode = Config::instance().load("input_mode", 0).toInt();
    modeCombo->setCurrentIndex(savedMode);

    layout->addRow("Input Detection", modeCombo);
    layout->addRow("Vertical Position:", ySlider);
    layout->addRow("Hide Duration (ms):", durationSlider);
    layout->addRow("", durationLabel); // Menampilkan angka ms
    layout->addRow("Text Color:", colorBtn);
    layout->addRow("Font Family:", fontBtn);

    connect(modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        Config::instance().save("input_mode", index);
        emit configChanged(); // Signal agar main.cpp/input_manager update
    });

    connect(ySlider, &QSlider::valueChanged, this, [this](int v) {
        Config::instance().save("y", v); emit configChanged();
    });

    connect(durationSlider, &QSlider::valueChanged, this, [this, durationLabel](int v) {
        durationLabel->setText(QString("%1 ms").arg(v));
        Config::instance().save("hide_duration", v);
        emit configChanged(); // Agar overlay tahu durasi berubah
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
