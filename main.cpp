#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    QFile file(":/style/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QString::fromUtf8(file.readAll());
        a.setStyleSheet(styleSheet);
    }
    w.setWindowIcon(QIcon(":/icons/album.png"));
    w.setWindowTitle(QObject::tr("相册"));
    w.show();
    return a.exec();
}
