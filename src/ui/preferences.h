#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>

class PreferencesWindow : public QDialog {
    Q_OBJECT
public:
    PreferencesWindow();

signals:
    void configChanged();

private:
    // Fungsi yang akan kita pecah ke preferences_ui.cpp
    void initWidgets();
    void setupLayout();
    
    // Fungsi yang tetap di preferences.cpp
    void setupConnections();
    void loadSettings();
    void updatePreview();

    // Widget members
    QLabel* previewLabel;
    QSlider *ySlider, *durationSlider;
    QLabel *durationLabel;
    QComboBox *backspaceCombo, *modeCombo;
    QLineEdit *mappingInput;
    QSpinBox *smartFormatSpin;
    QPushButton *colorBtn, *fontBtn, *selectAreaBtn;

    class ScreenSelector* currentSelector = nullptr;
};

#endif
