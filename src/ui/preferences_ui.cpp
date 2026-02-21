#include "preferences.h"
#include <QFormLayout>
#include <QVBoxLayout>

void PreferencesWindow::initWidgets() {
    ySlider = new QSlider(Qt::Horizontal);
    ySlider->setRange(10, 500);

    durationSlider = new QSlider(Qt::Horizontal);
    durationSlider->setRange(500, 10000);

    durationLabel = new QLabel();
    colorBtn = new QPushButton("Pick Color");
    fontBtn = new QPushButton("Pick Font");

    backspaceCombo = new QComboBox();
    backspaceCombo->addItem("Show Text [BackSpace]", 0);
    backspaceCombo->addItem("Delete Last Character", 1);

    modeCombo = new QComboBox();
    modeCombo->addItem("Character Mode (KeySym)", 0);
    modeCombo->addItem("Hardware Mode (KeyCode)", 1);

    mappingInput = new QLineEdit();
    mappingInput->setPlaceholderText("Contoh: {\"a\":\"1\", \"s\":\"2\"}");

    smartFormatSpin = new QSpinBox();
    smartFormatSpin->setRange(0, 10);
    smartFormatSpin->setSpecialValueText("Disabled");

    selectAreaBtn = new QPushButton("Select Display Area");
    
    previewLabel = new QLabel("Preview: [Ctrl+Alt+⏎]");
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setMinimumHeight(120);
}

void PreferencesWindow::setupLayout() {
    auto* mainLayout = new QVBoxLayout(this);
    auto* formLayout = new QFormLayout();

    formLayout->addRow("Overlay Position:", selectAreaBtn);
    formLayout->addRow("Smart Format Threshold:", smartFormatSpin);
    formLayout->addRow("Backspace Behavior:", backspaceCombo);
    formLayout->addRow("Custom Mapping (JSON):", mappingInput);
    formLayout->addRow("Input Detection:", modeCombo);
    formLayout->addRow("Bottom Offset (px):", ySlider);
    formLayout->addRow("Hide Duration (ms):", durationSlider);
    formLayout->addRow("", durationLabel);
    formLayout->addRow("Text Color:", colorBtn);
    formLayout->addRow("Font Family:", fontBtn);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(new QLabel("<b>Live Preview:</b>"));
    mainLayout->addWidget(previewLabel);
}
