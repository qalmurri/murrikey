#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVariant>
#include <QStyleOption>
#include <QStyle>

class PreferencesWindow : public QDialog {
    Q_OBJECT

public:
    PreferencesWindow();

signals:
    void configChanged();

private:
    void initWidgets();
    void setupLayout();
    void setupConnections();
    void loadSettings();
    void updatePreview();

    QLabel* previewLabel;
    QSlider *durationSlider; // ySlider dihapus
    QLabel *durationLabel;
    QComboBox *modeCombo;    // backspaceCombo dihapus
    QSpinBox *smartFormatSpin;
    QPushButton *colorBtn, *fontBtn, *selectAreaBtn;

    class ScreenSelector* currentSelector = nullptr;
};

#endif
