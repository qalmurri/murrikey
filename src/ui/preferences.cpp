#include "preferences.h"
#include "config.h"
#include <QFormLayout>
#include <QVBoxLayout> // Tambahkan ini
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

    // 1. Gunakan Layout Utama (Vertical)
    auto* mainLayout = new QVBoxLayout(this);
    auto* formLayout = new QFormLayout();

    auto* ySlider = new QSlider(Qt::Horizontal);
    ySlider->setRange(0, 1080);
    ySlider->setValue(Config::instance().load("y", 800).toInt());

    auto* durationSlider = new QSlider(Qt::Horizontal);
    durationSlider->setRange(500, 10000);
    durationSlider->setValue(Config::instance().load("hide_duration", 3000).toInt());

    auto* durationLabel = new QLabel(QString("%1 ms").arg(durationSlider->value()));
    auto* colorBtn = new QPushButton("Pick Color");
    auto* fontBtn = new QPushButton("Pick Font");

    auto* backspaceCombo = new QComboBox();
    backspaceCombo->addItem("Show Text [BackSpace]", 0);
    backspaceCombo->addItem("Delete Last Character", 1);
    backspaceCombo->setCurrentIndex(Config::instance().load("backspace_mode", 0).toInt());

    auto* modeCombo = new QComboBox();
    modeCombo->addItem("Character Mode (KeySym)", MODE_KEYSYM);
    modeCombo->addItem("Hardware Mode (KeyCode)", MODE_KEYCODE);
    modeCombo->setCurrentIndex(Config::instance().load("input_mode", MODE_KEYSYM).toInt());

    auto* mappingInput = new QLineEdit();
    mappingInput->setPlaceholderText("Contoh: {\"a\":\"1\", \"s\":\"2\"}");
    mappingInput->setText(Config::instance().load("custom_mapping", "{}").toString());

    // Tambahkan ke Form Layout
    formLayout->addRow("Backspace Behavior:", backspaceCombo);
    formLayout->addRow("Custom Mapping (JSON):", mappingInput);
    formLayout->addRow("Input Detection:", modeCombo);
    formLayout->addRow("Vertical Position:", ySlider);
    formLayout->addRow("Hide Duration (ms):", durationSlider);
    formLayout->addRow("", durationLabel);
    formLayout->addRow("Text Color:", colorBtn);
    formLayout->addRow("Font Family:", fontBtn);

    // 2. Tambahkan Preview Label di bagian bawah
    previewLabel = new QLabel("Preview: [Ctrl+Alt+⏎]");
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setMinimumHeight(120); // Beri ruang agar terlihat jelas

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(new QLabel("<b>Live Preview:</b>")); // Header kecil
    mainLayout->addWidget(previewLabel);

    // --- CONNECTS ---
    connect(this, &PreferencesWindow::configChanged, this, &PreferencesWindow::updatePreview);

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
        emit configChanged();
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

    // Jalankan preview pertama kali
    updatePreview();
}

// 3. Fungsi untuk Update Preview (Samakan dengan style overlay.cpp)
void PreferencesWindow::updatePreview() {
    QString c = Config::instance().load("color", "#00FF00").toString();
    QString f = Config::instance().load("font_family", "Monospace").toString();
    
    previewLabel->setStyleSheet(QString(
        "font-size: 36px; font-weight: bold; color: %1; font-family: '%2'; "
        "background-color: rgba(20, 20, 20, 220); padding: 20px; "
        "border-radius: 10px; border: 2px solid #333;"
    ).arg(c).arg(f));
}
