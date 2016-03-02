#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtAwesome.h>
#include <QString>
#include "settingswindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static MainWindow* instance();
    void askForMasterPassword();
    void openSettingsWindow();
    void closeSettingsWindow();
    void openSessionWindow(QString credentials = nullptr);
    void closeSessionWindow();
    void toggleBackgroundMode(bool flag, int wid = 0);
    QtAwesome *m_pAwesome;

private:
    static MainWindow *s_instance;
    Ui::MainWindow *ui;
    void mergeStyles();
    void enterForMasterPassword(SettingsWindow &settings);
};

#endif // MAINWINDOW_H
