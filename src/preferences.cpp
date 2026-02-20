#include "preferences.h"
#include "config.h"
#include <QFormLayout>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QFontDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QJsonDocument>
#include <QJsonObject>
PreferencesWindow::PreferencesWindow() {
    setWindowTitle("Murrikey Settings");
    auto* layout = new QFormLayout(this);
    auto* ySlider = new QSlider(Qt::Horizontal);
    ySlider->setRange(0, 1080);
    ySlider->setValue(Config::instance().load("y", 800).toInt());
    auto* durationSlider = new QSlider(Qt::Horizontal);
    durationSlider->setRange(500, 10000); // 0.5 detik sampai 10 detik
    durationSlider->setValue(Config::instance().load("hide_duration", 3000).toInt());
    auto* durationLabel = new QLabel(QString("%1 ms").arg(durationSlider->value()));
    auto* colorBtn = new QPushButton("Pick Color");
    auto* fontBtn = new QPushButton("Pick Font");
    auto* backspaceCombo = new QComboBox();
    backspaceCombo->addItem("Show Text [BackSpace]", 0);
    backspaceCombo->addItem("Delete Last Character", 1);
    auto* modeCombo = new QComboBox();
    modeCombo->addItem("Character Mode (KeySym)", MODE_KEYSYM);
    modeCombo->addItem("Hardware Mode (KeyCode)", MODE_KEYCODE);
    modeCombo->setCurrentIndex(Config::instance().load("input_mode", MODE_KEYSYM).toInt());
    int savedMode = Config::instance().load("input_mode", 0).toInt();
    auto* mappingInput = new QLineEdit();
    mappingInput->setPlaceholderText("Contoh: {\"a\":\"1\", \"s\":\"2\"}");
    mappingInput->setText(Config::instance().load("custom_mapping", "{}").toString());
    int bsMode = Config::instance().load("backspace_mode", 0).toInt();
    backspaceCombo->setCurrentIndex(bsMode);
    layout->addRow("Backspace Behavior:", backspaceCombo);
    layout->addRow("Custom Mapping (JSON):", mappingInput);
    layout->addRow("Input Detection", modeCombo);
    layout->addRow("Vertical Position:", ySlider);
    layout->addRow("Hide Duration (ms):", durationSlider);
    layout->addRow("", durationLabel); // Menampilkan angka ms
    layout->addRow("Text Color:", colorBtn);
    layout->addRow("Font Family:", fontBtn);
    connect(mappingInput, &QLineEdit::textChanged, this, [this](const QString& text) {
        QJsonParseError error;
        QJsonDocument::fromJson(text.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError || text.isEmpty()) {
            Config::instance().save("custom_mapping", text);
            emit configChanged();
        }
    });
    connect(modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        Config::instance().save("input_mode", index);
        emit configChanged(); 
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
    connect(backspaceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        Config::instance().save("backspace_mode", index);
        emit configChanged();
    });

}
