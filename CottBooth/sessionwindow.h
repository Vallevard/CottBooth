#ifndef SESSIONWINDOW_H
#define SESSIONWINDOW_H

#include <QWidget>
#include "session.h"

namespace Ui {
class SessionWindow;
}

class SessionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SessionWindow(QWidget *parent = 0);
    ~SessionWindow();

    bool openSession(QString sid = NULL);

private:
    Ui::SessionWindow *ui;
    Session *m_pSession;

    bool showCredentialDialog();
    void initCamera();

protected:
    //void showEvent(QShowEvent *event);

};

#endif // SESSIONWINDOW_H
