#include "appsettings.h"

AppSettings::AppSettings() :
    Ms::Core::MSettingsBase()
{
}

void AppSettings::loadAppSettings()
{
    boost::property_tree::ptree pt;

    //General settings
    pt = loadSettings(m_settingsFileName);

    //General settings
    m_loadInactiveData = pt.get<bool>("Database.LoadInactiveData");
    m_showExtraColumns = pt.get<bool>("Database.ShowExtraColumns");
    m_dataDir = pt.get<std::string>("General.DataDir");

    //Database settings
    m_databaseBackEnd = pt.get<std::string>("Database.Connection.Backend");
    m_databaseHost = pt.get<std::string>("Database.Connection.Host");
    m_databasePort = pt.get<std::string>("Database.Connection.Port");
    m_databaseSchema = pt.get<std::string>("Database.Connection.Schema");
    m_databaseUsername = pt.get<std::string>("Database.Connection.Username");
    m_databasePassword = pt.get<std::string>("Database.Connection.Password");
}

void AppSettings::initDefaultSettings()
{
    boost::property_tree::ptree pt;

    //General settings
    m_loadInactiveData = true;
    m_showExtraColumns = true;
    m_dataDir = "data";

    //Database settings
    m_databaseBackEnd = "MySQL";
    m_databaseHost = "127.0.0.1";
    m_databasePort = "3306";
    m_databaseSchema = "MercurySteam";
    m_databaseUsername = "root";
    m_databasePassword = "";

    pt.put<bool>("Database.LoadInactiveData", m_loadInactiveData);
    pt.put<bool>("Database.ShowExtraColumns", m_showExtraColumns);
    pt.put<std::string>("General.DataDir", m_dataDir);

    pt.put<std::string>("Database.Connection.Backend", m_databaseBackEnd);
    pt.put<std::string>("Database.Connection.Host", m_databaseHost);
    pt.put<std::string>("Database.Connection.Port", m_databasePort);
    pt.put<std::string>("Database.Connection.Schema", m_databaseSchema);
    pt.put<std::string>("Database.Connection.Username", m_databaseUsername);
    pt.put<std::string>("Database.Connection.Password", m_databasePassword);

    saveSettings(pt);
}

bool AppSettings::isLoadInactiveData() const
{
    return m_loadInactiveData;
}

void AppSettings::setLoadInactiveData(bool value)
{
    m_loadInactiveData = value;

    putSetting<bool>("Database.LoadInactiveData", value);
}

bool AppSettings::isShowExtraColumns() const
{
    return m_showExtraColumns;
}

void AppSettings::setShowExtraColumns(bool value)
{
    m_showExtraColumns = value;

    putSetting<bool>("Database.ShowExtraColumns", value);
}

std::string AppSettings::dataDir() const
{
    return m_dataDir;
}

void AppSettings::setDataDir(const std::string &dataDir)
{
    m_dataDir = dataDir;

    putSetting<std::string>("General.DataDir", dataDir);
}

std::string AppSettings::databaseBackEnd() const
{
    return m_databaseBackEnd;
}

void AppSettings::setDatabaseBackEnd(const std::string &value)
{
    m_databaseBackEnd = value;

    putSetting<std::string>("Database.Connection.BackEnd", value);
}

std::string AppSettings::databaseHost() const
{
    return m_databaseHost;
}

void AppSettings::setDatabaseHost(const std::string &value)
{
    m_databaseHost = value;

    putSetting<std::string>("Database.Connection.Host", value);
}

std::string AppSettings::databasePort() const
{
    return m_databasePort;
}

void AppSettings::setDatabasePort(const std::string &value)
{
    m_databasePort = value;

    putSetting<std::string>("Database.Connection.Port", value);
}

std::string AppSettings::databaseSchema() const
{
    return m_databaseSchema;
}

void AppSettings::setDatabaseSchema(const std::string &value)
{
    m_databaseSchema = value;

    putSetting<std::string>("Database.Connection.Schema", value);
}

std::string AppSettings::databaseUsername() const
{
    return m_databaseUsername;
}

void AppSettings::setDatabaseUsername(const std::string &value)
{
    m_databaseUsername = value;

    putSetting<std::string>("Database.Connection.UserName", value);
}

std::string AppSettings::databasePassword() const
{
    return m_databasePassword;
}

void AppSettings::setDatabasePassword(const std::string &value)
{
    m_databasePassword = value;

    putSetting<std::string>("Database.Connection.Password", value);
}

std::string AppSettings::appRoot() const
{
    return m_appRoot;
}

void AppSettings::setAppRoot(const std::string &path)
{
    m_appRoot = path;
}

std::string AppSettings::docRoot() const
{
    return m_docRoot;
}

void AppSettings::setDocRoot(const std::string &path)
{
    m_docRoot = path;
}
