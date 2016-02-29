#include "mainwindow.h"
#include "settings.h"
#include <QApplication>
#include <QtAwesome.h>
#include <QLineEdit>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    
    MainWindow w;    
    QString fontFamily = Settings::instance()->stringValue(Settings::THEME_FONT);
    int fontSize = Settings::instance()->intValue(Settings::THEME_FONT_SIZE);
    QFont fnt(fontFamily, fontSize);
    a.setFont(fnt);
    //w.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    w.showMaximized();
    return a.exec();
}
