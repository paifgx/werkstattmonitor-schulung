#include "hmiwindow.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    const QString path = app.arguments().size() > 1
        ? app.arguments().at(1)
        : QDir(QCoreApplication::applicationDirPath()).filePath("data/normal.csv");
    HmiWindow window(path);
    window.show();
    return app.exec();
}
