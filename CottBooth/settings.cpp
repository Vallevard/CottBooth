#include "settings.h"
#include <QApplication>

Settings* Settings::s_pSettings = nullptr;

Settings *Settings::instance()
{
    if(s_pSettings == nullptr)
    {
        s_pSettings = new Settings();
    }

    return s_pSettings;
}

Settings::Settings()
    : m_keyMap()
{
    QString settingsFile = QApplication::applicationDirPath() + "/cottbooth_settings.ini";
    m_pSettings = new QSettings(settingsFile, QSettings::IniFormat);
    m_pSettings->setIniCodec("UTF-8");

    m_keyMap.insert(FIRST_RUN, "main/first_run");
    m_keyMap.insert(SAVE_PATH, "main/save_path");
    m_keyMap.insert(ASK_FOR_SAVE_PATH, "main/ask_for_save_path");
    m_keyMap.insert(SELECT_LAST_PIC, "main/select_last_picture");
    m_keyMap.insert(CREDENTIAL_LENGTH, "main/credential_length");

    m_keyMap.insert(LIVE_VIEW, "camera/live_view");
    m_keyMap.insert(APERTURE, "camera/aperture");
    m_keyMap.insert(SHUTTER_SPEED, "camera/shutter_speed");
    m_keyMap.insert(WHITE_BALANCE, "camera/white_balance");

    m_keyMap.insert(MASTER_PASSWORD, "security/master");
    m_keyMap.insert(RESPAWN_TOKEN, "security/respawn_token");

    m_keyMap.insert(THEME, "theme/name");
    m_keyMap.insert(THEME_FONT, "theme/font");
    m_keyMap.insert(THEME_FONT_SIZE, "theme/default_size");

    if(boolValue(FIRST_RUN, true))
    {
        initDefaults();
    }
}

void Settings::initDefaults()
{
    setStringValue(SAVE_PATH, "");
    setBoolValue(ASK_FOR_SAVE_PATH, false);
    setBoolValue(SELECT_LAST_PIC, true);
    setIntValue(CREDENTIAL_LENGTH, 8);

    setBoolValue(LIVE_VIEW, true);
    setStringValue(APERTURE, "");
    setStringValue(SHUTTER_SPEED, "");
    setStringValue(WHITE_BALANCE, "");

    setStringValue(MASTER_PASSWORD, "");
    setStringValue(RESPAWN_TOKEN, "");

    setStringValue(THEME, "Default");
    setStringValue(THEME_FONT, "Ubuntu");
    setIntValue(THEME_FONT_SIZE, 16);
}

bool Settings::boolValue(BoolValues key, bool defaultValue)
{
    return m_pSettings->value(m_keyMap.value(key), defaultValue).toBool();
}

QString Settings::stringValue(StringValues key, QString defaultValue)
{
    return m_pSettings->value(m_keyMap.value(key), defaultValue).toString();
}

int Settings::intValue(IntValues key, int defaultValue)
{
    return m_pSettings->value(m_keyMap.value(key), defaultValue).toInt();
}

void Settings::setBoolValue(BoolValues key, bool value)
{
    m_pSettings->setValue(m_keyMap.value(key), value);
}

void Settings::setIntValue(IntValues key, int value)
{
    m_pSettings->setValue(m_keyMap.value(key), value);
}

void Settings::setStringValue(StringValues key, QString value)
{
    m_pSettings->setValue(m_keyMap.value(key), value);
}
