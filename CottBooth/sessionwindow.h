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
    QList<QObject> *m_pImageList;

    bool showCredentialDialog();
    bool validateCredentials();
    void initCamera();
    void loadImages();


protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void imageLoaded(QPixmap pixMap, int x, int y);
};

#endif // SESSIONWINDOW_H
