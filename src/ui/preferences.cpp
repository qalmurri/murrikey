#include "preferences.h"
#include "config.h"
#include "screen_selector.h"
#include <QColorDialog>
#include <QFontDialog>

PreferencesWindow::PreferencesWindow() {
    setWindowTitle("Murrikey Settings");
    initWidgets();      
    setupLayout();      
    loadSettings();     
    setupConnections(); 
    updatePreview();
}

void PreferencesWindow::loadSettings() {
    durationSlider->setValue(Config::instance().load("hide_duration", 3000).toInt());
    durationLabel->setText(QString("%1 ms").arg(durationSlider->value()));
    modeCombo->setCurrentIndex(Config::instance().load("input_mode", 0).toInt());
    smartFormatSpin->setValue(Config::instance().load("smart_format_threshold", 2).toInt());
}

void PreferencesWindow::setupConnections() {
    connect(selectAreaBtn, &QPushButton::clicked, this, [this]() {
        if (currentSelector) { currentSelector->raise(); return; }
        currentSelector = new ScreenSelector();
        connect(currentSelector, &QObject::destroyed, this, [this]() { currentSelector = nullptr; });
        connect(currentSelector, &ScreenSelector::areaSelected, this, [this](QRect rect) {
            Config::instance().save("x", rect.x());
            Config::instance().save("y", rect.y());
            Config::instance().save("width", rect.width());
            Config::instance().save("height", rect.height());
            emit configChanged();
        });
        currentSelector->show();
    });

    connect(colorBtn, &QPushButton::clicked, this, [this]() {
        QColor c = QColorDialog::getColor(Qt::green, this);
        if (c.isValid()) { Config::instance().save("color", c.name()); emit configChanged(); }
    });

    connect(fontBtn, &QPushButton::clicked, this, [this]() {
        bool ok; QFont f = QFontDialog::getFont(&ok, QFont("Monospace", 36), this);
        if (ok) { Config::instance().save("font_family", f.family()); emit configChanged(); }
    });

    connect(durationSlider, &QSlider::valueChanged, this, [this](int v) {
        Config::instance().save("hide_duration", v);
        durationLabel->setText(QString("%1 ms").arg(v));
        emit configChanged();
    });

    connect(smartFormatSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int v) {
        Config::instance().save("smart_format_threshold", v); emit configChanged();
    });

    connect(this, &PreferencesWindow::configChanged, this, &PreferencesWindow::updatePreview);
}

void PreferencesWindow::updatePreview() {
    QString c = Config::instance().load("color", "#00FF00").toString();
    QString f = Config::instance().load("font_family", "Monospace").toString();
    previewLabel->setStyleSheet(QString(
        "font-size: 36px; font-weight: bold; color: %1; font-family: '%2'; "
        "background-color: rgba(20, 20, 20, 220); padding: 20px; "
        "border-radius: 10px; border: 2px solid #333;"
    ).arg(c).arg(f));
}
