#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "mainwindow.h"
#include "settings.h"
#include <QDebug>
#include <QCryptographicHash>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QStorageInfo>
#include <QUuid>

#define DIALOG_MASK Qt::Tool|Qt::FramelessWindowHint

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    ui->btnBack->setFont(MainWindow::instance()->m_pAwesome->font(32));
    ui->btnBack->setText( QString( fa::backward ) );    

    QListWidgetItem *general = new QListWidgetItem(ui->listTopics);
    QListWidgetItem *camera = new QListWidgetItem(ui->listTopics);
    QListWidgetItem *security = new QListWidgetItem(ui->listTopics);
    QListWidgetItem *design = new QListWidgetItem(ui->listTopics);

    general->setSizeHint(QSize(0,64));
    general->setIcon(MainWindow::instance()->m_pAwesome->icon(fa::wrench));
    general->setText(tr("Main"));
    general->setSelected(true);

    camera->setSizeHint(QSize(0, 64));
    camera->setIcon(MainWindow::instance()->m_pAwesome->icon(fa::cameraretro));
    camera->setText(tr("Camera"));

    security->setSizeHint(QSize(0, 64));
    security->setIcon(MainWindow::instance()->m_pAwesome->icon(fa::key));
    security->setText(tr("Security"));

    design->setSizeHint(QSize(0, 64));
    design->setIcon(MainWindow::instance()->m_pAwesome->icon(fa::desktop));
    design->setText(tr("Design"));

    ui->listTopics->insertItem(0, general);
    ui->listTopics->insertItem(1, security);
    ui->listTopics->insertItem(2, camera);
    ui->listTopics->insertItem(3, design);

    ui->listTopics->setIconSize(QSize(32,32));    
    ui->listTopics->setAttribute(Qt::WA_MacShowFocusRect, 0);    
    ui->sbFontSize->findChild<QLineEdit*>()->setReadOnly(true);
    ui->sbFontSize->setAttribute(Qt::WA_MacShowFocusRect, 0);

    QVector<QString> themes = scanThemeFolder(QApplication::applicationDirPath() + "/themes");
    ui->cbTheme->addItems(themes.toList());

    qDebug() << "drives: " << QDir::drives().size();

    ui->btnBrowse->setFont(MainWindow::instance()->m_pAwesome->font(28));
    ui->btnBrowse->setText( QString( fa::folderopeno ) );

    this->loadSettings();
    this->initEventHandling();

    qDebug() << "Random: " << QUuid::createUuid().toString();

    /*
    for (auto volume : QStorageInfo::mountedVolumes()) {
            qDebug() << "Name:" << volume.name();
            qDebug() << "Display name:" << volume.displayName();
            qDebug() << "Device:" << volume.device();
            qDebug() << "Root path:" << volume.rootPath();
            qDebug() << "File system type:" << volume.fileSystemType();
            qDebug() << "Is valid?" << (volume.isValid() ? "yes" : "no");
            qDebug() << "Is root?" << (volume.isRoot() ? "yes" : "no");
            qDebug() << "Is ready?" << (volume.isReady() ? "yes" : "no");
            qDebug() << "Is read only?" << (volume.isReadOnly() ? "yes" : "no");
            qDebug() << "Bytes available:" << volume.bytesAvailable();
            qDebug() << "Bytes free:" << volume.bytesFree();
            qDebug() << "Bytes total:" << volume.bytesTotal() << endl;
        } */
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::initEventHandling()
{
    connect(ui->listTopics, &QListWidget::itemClicked,[=](QListWidgetItem *item){
       ui->settingsStack->setCurrentIndex(ui->listTopics->row(item));
    });

    connect(ui->btnBack,&QToolButton::clicked, [=](){
        MainWindow::instance()->closeSettingsWindow();
    });

    connect(ui->btnBrowse, &QToolButton::clicked, [=](){
        MainWindow::instance()->toggleBackgroundMode(true, 1);
        QFileDialog fileDialog(this, DIALOG_MASK);
        fileDialog.setOptions(QFileDialog::ShowDirsOnly|QFileDialog::ReadOnly|QFileDialog::DontUseNativeDialog);
        fileDialog.setFileMode(QFileDialog::DirectoryOnly);
        if ( fileDialog.exec() )
        {
            QString path = fileDialog.selectedFiles().first();
            ui->lblPath->setText(path);
            Settings::instance()->setStringValue(Settings::SAVE_PATH, path);
        }

        MainWindow::instance()->toggleBackgroundMode(false, 1);
    });


    connect(ui->cbAskSavePath, &QCheckBox::stateChanged, this, &SettingsWindow::handleCheckBoxEvent);
    connect(ui->cbSelectLastPicture, &QCheckBox::stateChanged, this, &SettingsWindow::handleCheckBoxEvent);
    connect(ui->cbLiveView, &QCheckBox::stateChanged, this, &SettingsWindow::handleCheckBoxEvent);

    connect(ui->cbAperture, &QComboBox::currentTextChanged, this, &SettingsWindow::handleComboBoxEvent);
    connect(ui->cbShutterSpeed, &QComboBox::currentTextChanged, this, &SettingsWindow::handleComboBoxEvent);
    connect(ui->cbWhiteBalance, &QComboBox::currentTextChanged, this, &SettingsWindow::handleComboBoxEvent);

    connect(ui->tbMasterPassword, &QLineEdit::returnPressed, this, &SettingsWindow::handleTextBoxEnter);
    connect(ui->tbRespawnToken, &QLineEdit::returnPressed, this, &SettingsWindow::handleTextBoxEnter);

    connect(ui->cbTheme, &QComboBox::currentTextChanged, this, &SettingsWindow::handleComboBoxEvent);
    connect(ui->cbFont, &QFontComboBox::currentTextChanged, this, &SettingsWindow::handleComboBoxEvent);
    connect(ui->sbFontSize, SIGNAL(valueChanged(int)), this, SLOT(handleSpinBoxEvent(int)));
    //TODO: find out whats wrong with this line
    //connect(ui->sbFontSize, &QSpinBox::valueChanged, this, &SettingsWindow::handleSpinBoxEvent);

}

void SettingsWindow::loadSettings()
{
    Settings *s = Settings::instance();
    ui->lblPath->setText(s->stringValue(Settings::SAVE_PATH));
    ui->cbAskSavePath->setChecked(s->boolValue(Settings::ASK_FOR_SAVE_PATH, true));
    ui->cbSelectLastPicture->setChecked(s->boolValue(Settings::SELECT_LAST_PIC, true));

    ui->cbLiveView->setChecked(s->boolValue(Settings::LIVE_VIEW, false));
    ui->cbAperture->setCurrentIndex( ui->cbAperture->findText(s->stringValue(Settings::APERTURE)) );
    ui->cbShutterSpeed->setCurrentIndex( ui->cbShutterSpeed->findText(s->stringValue(Settings::SHUTTER_SPEED)) );
    ui->cbWhiteBalance->setCurrentIndex( ui->cbWhiteBalance->findText(s->stringValue(Settings::WHITE_BALANCE)) );

    ui->cbTheme->setCurrentIndex( ui->cbTheme->findText(s->stringValue(Settings::THEME, "Default")) );
    ui->cbFont->setCurrentIndex( ui->cbFont->findText(s->stringValue(Settings::THEME_FONT, "Ubuntu")) );
    ui->sbFontSize->setValue( s->intValue(Settings::THEME_FONT_SIZE, 16));
}

void SettingsWindow::saveEncryptedPassword(QString passwd)
{
    QString hash = QString(QCryptographicHash::hash(passwd.toUtf8(), QCryptographicHash::Sha256).toHex());
    Settings::instance()->setStringValue(Settings::MASTER_PASSWORD, hash);
    Settings::instance()->setBoolValue(Settings::FIRST_RUN, false);
}

QVector<QString> SettingsWindow::scanThemeFolder(QString folder)
{
    QVector<QString> themes = QVector<QString>();
    QDir dir(folder);
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::Dirs);


    for(int i = 0; i < list.count(); i++)
    {
        QFileInfo info = list[i];
        if(info.isDir())
        {
            QDir themeDir(info.filePath());
            QFileInfoList themeList = themeDir.entryInfoList(QDir::NoDotAndDotDot|QDir::Files);
            for(int j = 0; j < themeList.count(); j++)
            {
                QFileInfo themeInfo = themeList[j];
                if(themeInfo.isFile() && themeInfo.fileName().contains(".css")
                        && themeInfo.baseName().compare(info.baseName()) == 0)
                {
                    themes.append(info.fileName());
                }
            }
        }
    }

    return themes;
}

void SettingsWindow::handleCheckBoxEvent(int state)
{
    Settings *s = Settings::instance();
    QString objName = qobject_cast<QCheckBox *>(sender())->objectName();
    if(objName == "cbAskSavePath")
    {
        s->setBoolValue(Settings::ASK_FOR_SAVE_PATH, static_cast<bool>(state));
    }
    else if (objName == "cbSelectLastPicture")
    {
        s->setBoolValue(Settings::SELECT_LAST_PIC, static_cast<bool>(state));
    }
    else if (objName == "cbLiveView")
    {
        s->setBoolValue(Settings::LIVE_VIEW, static_cast<bool>(state));
    }
}

void SettingsWindow::handleComboBoxEvent(QString val)
{
    Settings *s = Settings::instance();
    QComboBox *cb = qobject_cast<QComboBox *>(sender());
    QString objName = cb->objectName();

    if(objName == "cbAperture")
    {
        s->setStringValue(Settings::APERTURE, val);
    }
    else if (objName == "cbShutterSpeed")
    {
        s->setStringValue(Settings::SHUTTER_SPEED, val);
    }
    else if (objName == "cbWhiteBalance")
    {
        s->setStringValue(Settings::WHITE_BALANCE, val);
    }
    else if (objName == "cbTheme")
    {
        s->setStringValue(Settings::THEME, val);
    }
    else if (objName == "cbFont")
    {
        s->setStringValue(Settings::THEME_FONT, val);
    }
}

void SettingsWindow::handleTextBoxEnter()
{
    Settings *s = Settings::instance();
    QLineEdit *tb = qobject_cast<QLineEdit *>(sender());
    QString objName = tb->objectName();

    if (tb->text().isEmpty())
    {
        MainWindow::instance()->toggleBackgroundMode(true, 1);
        QMessageBox alert(QMessageBox::Critical, tr("Error"), tr("Empty values are not allowed"), QMessageBox::Ok, this, DIALOG_MASK);
        alert.exec();
        MainWindow::instance()->toggleBackgroundMode(false, 1);
        return;
    }

    tb->clearFocus();

    if(objName == "tbSavePath")
    {
        s->setStringValue(Settings::SAVE_PATH, tb->text());
    }
    else if (objName == "tbMasterPassword")
    {
        QString hash = QString(QCryptographicHash::hash(tb->text().toUtf8(), QCryptographicHash::Sha256).toHex());
        s->setStringValue(Settings::MASTER_PASSWORD, hash);
        tb->clear();
    }
    else if (objName == "tbRespawnToken")
    {
        QString hash = QString(QCryptographicHash::hash(tb->text().toUtf8(), QCryptographicHash::Sha256).toHex());
        s->setStringValue(Settings::RESPAWN_TOKEN, hash);
        tb->clear();
    }
}

void SettingsWindow::handleSpinBoxEvent(int val)
{
    Settings *s = Settings::instance();
    QString objName = qobject_cast<QSpinBox *>(sender())->objectName();

    if(objName == "sbFontSize")
    {
        s->setIntValue(Settings::THEME_FONT_SIZE, val);
    }

}
