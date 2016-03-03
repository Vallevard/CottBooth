#ifndef SESSIONWINDOW_H
#define SESSIONWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include "session.h"
#include "image.h"

#define THUMB_WIDTH 160
#define THUMB_HEIGHT 120

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
    QList<Image*> *m_pImageList;
    QGridLayout *m_pImageGrid;
    uint m_uiColumns;

    bool showCredentialDialog();
    bool validateCredentials();
    void initCamera();
    void loadImages();


protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void imageLoaded(Image *img);
    void imageClicked();
};

#endif // SESSIONWINDOW_H

