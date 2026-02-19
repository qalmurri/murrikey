#include <QApplication>
#include <QSystemTrayIcon>
#include <QFontDialog>
#include <QFont>
#include <QMenu>
#include <QStyle>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSettings>
#include <QDialog>
#include <QSlider>
#include <QColorDialog>
#include <QPushButton>
#include <QFormLayout>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>

// --- KELAS PREFERENCES ---
class PreferencesWindow : public QDialog {
    Q_OBJECT
public:
    PreferencesWindow(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Settings");
        QFormLayout *layout = new QFormLayout(this); // Gunakan nama 'layout'
        
        QSettings s("MyProject", "Screenkey");

        // Slider Posisi
        QSlider *ySlider = new QSlider(Qt::Horizontal);
        ySlider->setRange(0, 1080);
        ySlider->setValue(s.value("y", 800).toInt());

        // Tombol Warna
        QPushButton *colorBtn = new QPushButton("Pilih Warna");
        
        // Tombol Font
        QPushButton *fontBtn = new QPushButton("Pilih Jenis Font");
        
        layout->addRow("Posisi Vertikal:", ySlider);
        layout->addRow("Warna Teks:", colorBtn);
        layout->addRow("Font Family:", fontBtn);
        
        // Koneksi Signal
        connect(ySlider, &QSlider::valueChanged, this, [this](int val) {
            emit positionChanged(val);
            QSettings s("MyProject", "Screenkey");
            s.setValue("y", val);
        });

        connect(colorBtn, &QPushButton::clicked, this, [this]() {
            QColor color = QColorDialog::getColor(Qt::green, this);
            if (color.isValid()) {
                emit colorChanged(color.name());
                QSettings s("MyProject", "Screenkey");
                s.setValue("color", color.name());
            }
        });

        connect(fontBtn, &QPushButton::clicked, this, [this]() {
            bool ok;
            QFont font = QFontDialog::getFont(&ok, QFont("Monospace", 36), this);
            if (ok) {
                emit fontChanged(font.family());
                QSettings s("MyProject", "Screenkey");
                s.setValue("font_family", font.family());
            }
        });
    }

signals: // Signal diletakkan di luar fungsi
    void fontChanged(QString fontFamily);
    void positionChanged(int y);
    void colorChanged(QString color);
};

// --- KELAS OVERLAY ---
class ScreenkeyOverlay : public QWidget {
public:
    ScreenkeyOverlay() {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::WindowTransparentForInput);
        setAttribute(Qt::WA_TranslucentBackground);
        
        label = new QLabel("", this);
        label->setAlignment(Qt::AlignCenter);
        
        opacityEffect = new QGraphicsOpacityEffect(this);
        label->setGraphicsEffect(opacityEffect);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(label);

        display = XOpenDisplay(NULL);
        
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &ScreenkeyOverlay::checkInput);
        timer->start(10);

        fadeTimer = new QTimer(this);
        fadeTimer->setSingleShot(true);
        connect(fadeTimer, &QTimer::timeout, this, &ScreenkeyOverlay::startFadeOut);

        resize(800, 120);
        loadSettings();
    }

    // Hanya satu fungsi loadSettings
    void loadSettings() {
        QSettings s("MyProject", "Screenkey");
        int y = s.value("y", 800).toInt();
        QString color = s.value("color", "#00FF00").toString();
        QString fontFamily = s.value("font_family", "Monospace").toString();
        
        move(400, y);
        updateStyle(color, fontFamily);
    }

    // Hanya satu fungsi updateStyle yang mendukung font
    void updateStyle(QString color, QString fontFamily) {
        label->setStyleSheet(QString(
            "font-size: 36px; font-weight: bold; color: %1; "
            "font-family: '%2'; "
            "background-color: rgba(20, 20, 20, 200); "
            "padding: 20px; border-radius: 10px; border: 2px solid #333;"
        ).arg(color).arg(fontFamily));
    }

private:
    QLabel *label;
    Display *display;
    char old_keys[32] = {0};
    QString currentBuffer = "";
    QGraphicsOpacityEffect *opacityEffect;
    QTimer *fadeTimer;

    bool checkModifier(KeySym sym) {
        return (sym == XK_Control_L || sym == XK_Control_R || sym == XK_Shift_L || 
                sym == XK_Shift_R || sym == XK_Alt_L || sym == XK_Alt_R);
    }

    void checkInput() {
        char keys[32];
        XQueryKeymap(display, keys);
        XkbStateRec state;
        XkbGetState(display, XkbUseCoreKbd, &state);
        
        bool ctrl = state.mods & ControlMask;
        bool shift = state.mods & ShiftMask;
        bool alt = state.mods & Mod1Mask;
        bool caps = state.mods & LockMask;

        for (int i = 0; i < 32; i++) {
            if (keys[i] != old_keys[i]) {
                for (int bit = 0; bit < 8; bit++) {
                    if ((keys[i] & (1 << bit)) && !(old_keys[i] & (1 << bit))) {
                        int keycode = i * 8 + bit;
                        KeySym sym = XkbKeycodeToKeysym(display, keycode, 0, (shift ^ caps) ? 1 : 0);
                        if (checkModifier(sym)) continue;
                        char *name = XKeysymToString(sym);
                        if (name) processKey(QString(name), ctrl, shift, alt);
                    }
                }
            }
        }
        memcpy(old_keys, keys, 32);
    }

    void processKey(QString keyName, bool ctrl, bool shift, bool alt) {
        if (keyName == "space") keyName = " ";
        else if (keyName == "Return") keyName = "⏎";
        else if (keyName == "BackSpace") { if(!currentBuffer.isEmpty()) currentBuffer.chop(1); showText(); return; }
        else if (keyName == "plus") keyName = "+";
        else if (keyName == "equal") keyName = "=";

        QString fullKey = "";
        if (ctrl) fullKey += "Ctrl+";
        if (alt) fullKey += "Alt+";
        if (shift && keyName.length() > 1) fullKey += "Shift+";
        fullKey += keyName;

        if (ctrl || alt || keyName.length() > 1) currentBuffer += " [" + fullKey + "] ";
        else currentBuffer += keyName;

        if (currentBuffer.length() > 30) currentBuffer = currentBuffer.right(30);
        showText();
    }

    void showText() { 
        label->setText(currentBuffer); 
        opacityEffect->setOpacity(1.0); 
        fadeTimer->start(3000); 
    }

    void startFadeOut() { 
        QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
        animation->setDuration(800);
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);
        connect(animation, &QPropertyAnimation::finished, [this]() {
            currentBuffer = ""; 
        });
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
};

// --- MAIN ---
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    ScreenkeyOverlay overlay;
    PreferencesWindow prefs;

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(&app);
    trayIcon->setIcon(app.style()->standardIcon(QStyle::SP_ComputerIcon));
    
    QMenu *trayMenu = new QMenu();
    QAction *prefsAction = trayMenu->addAction("Preferences");
    QAction *quitAction = trayMenu->addAction("Quit");
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    QObject::connect(prefsAction, &QAction::triggered, &prefs, &PreferencesWindow::show);
    QObject::connect(quitAction, &QAction::triggered, &app, &QCoreApplication::quit);

    QObject::connect(&prefs, &PreferencesWindow::positionChanged, &overlay, [&overlay](int y) {
        overlay.move(overlay.x(), y);
    });
    
    QObject::connect(&prefs, &PreferencesWindow::colorChanged, &overlay, [&overlay](QString color) {
        QSettings s("MyProject", "Screenkey");
        QString font = s.value("font_family", "Monospace").toString();
        overlay.updateStyle(color, font);
    });

    QObject::connect(&prefs, &PreferencesWindow::fontChanged, &overlay, [&overlay](QString fontFamily) {
        QSettings s("MyProject", "Screenkey");
        QString color = s.value("color", "#00FF00").toString();
        overlay.updateStyle(color, fontFamily);
    });

    overlay.show();
    return app.exec();
}

#include "main.moc"
