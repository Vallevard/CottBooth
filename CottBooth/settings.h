#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QHash>

namespace CottBooth {
class Settings;
}

class Settings
{    
public:
    enum BoolValues
    {
        FIRST_RUN = 0x0,
        ASK_FOR_SAVE_PATH = 0x1,
        SELECT_LAST_PIC = 0x2,
        LIVE_VIEW = 0x3,
        KEEP_IMAGE_ON_CAMERA = 0x4
    };

    enum StringValues
    {
        SAVE_PATH = 0x10,
        MASTER_PASSWORD = 0x11,
        RESPAWN_TOKEN = 0x12,
        THEME = 0x13,
        THEME_FONT = 0x14,
        APERTURE = 0x15,
        SHUTTER_SPEED = 0x16,
        WHITE_BALANCE = 0x17
    };

    enum IntValues
    {
        THEME_FONT_SIZE = 0x50,
        CREDENTIAL_LENGTH = 0x51
    };

public:
    static Settings *instance();
    bool boolValue(BoolValues key, bool defaultValue = false);
    int intValue(IntValues key, int defaultValue = 0);
    QString stringValue(StringValues key, QString defaultValue = nullptr);

    void setBoolValue(BoolValues key, bool value);
    void setIntValue(IntValues key, int value);
    void setStringValue(StringValues key, QString value);

private:
    QHash<int, QString> m_keyMap;
    QSettings *m_pSettings;
    static Settings *s_pSettings;
    Settings();

    void initDefaults();
};

#endif // SETTINGS_H
