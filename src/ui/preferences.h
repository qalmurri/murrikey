#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QVariant>       // WAJIB ADA
#include <QStyleOption>    // UNTUK FIX QStyleOptionFrame / QStyleOptionButton
#include <QStyle> // Tambahkan ini
                  //
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
    QSlider *ySlider, *durationSlider;
    QLabel *durationLabel;
    QComboBox *backspaceCombo, *modeCombo;
    QLineEdit *mappingInput;
    QSpinBox *smartFormatSpin;
    QPushButton *colorBtn, *fontBtn, *selectAreaBtn;

    class ScreenSelector* currentSelector = nullptr;
};

#endif
