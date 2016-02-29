#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QVector>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();
    void saveEncryptedPassword(QString passwd);    

private:
    Ui::SettingsWindow *ui;
    QVector<QString> scanThemeFolder(QString folder);

    void initEventHandling();
    void loadSettings();

private slots:
    void handleComboBoxEvent(QString val);
    void handleCheckBoxEvent(int state);
    void handleSpinBoxEvent(int val);
    void handleTextBoxEnter();
};

#endif // SETTINGSWINDOW_H
