#include <QApplication>
#include "app.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    App murrikey;
    murrikey.start();

    return app.exec();
}
