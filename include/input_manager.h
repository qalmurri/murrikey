#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <QObject>
#include <QString>

// Jangan include X11 di sini!
// Gunakan pointer mentah untuk menghindari ketergantungan header
typedef struct _XDisplay Display; 

class InputManager : public QObject {
    Q_OBJECT
public:
    InputManager();
    ~InputManager();
    void check();

signals:
    void keyPressed(QString name, bool ctrl, bool shift, bool alt);

private:
    Display* display;
    char old_keys[32];
    // Ganti KeySym (tipe data X11) dengan unsigned long agar tidak butuh header X11
    bool isModifier(unsigned long s); 
};

#endif
