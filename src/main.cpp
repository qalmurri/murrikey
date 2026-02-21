#include <QApplication>
#include "murrikey_app.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    MurrikeyApp murrikey;
    murrikey.start();

    return app.exec();
}
