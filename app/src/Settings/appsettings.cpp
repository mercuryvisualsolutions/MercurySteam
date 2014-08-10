#include "appsettings.h"

AppSettings::AppSettings() :
    Ms::Core::MSettingsBase()
{
}

void AppSettings::loadAppSettings()
{
    boost::property_tree::ptree pt;

    //General settings
    pt = loadSettings(settingsFileName_);

    //General settings
    _loadInactiveData = pt.get<bool>("Database.LoadInactiveData");
    _showExtraColumns = pt.get<bool>("Database.ShowExtraColumns");
    _dataDir = pt.get<std::string>("General.DataDir");

    //Database settings
    _databaseBackEnd = pt.get<std::string>("Database.Connection.Backend");
    _databaseHost = pt.get<std::string>("Database.Connection.Host");
    _databasePort = pt.get<std::string>("Database.Connection.Port");
    _databaseSchema = pt.get<std::string>("Database.Connection.Schema");
    _databaseUsername = pt.get<std::string>("Database.Connection.Username");
    _databasePassword = pt.get<std::string>("Database.Connection.Password");
}

void AppSettings::initDefaultSettings()
{
    boost::property_tree::ptree pt;

    //General settings
    _loadInactiveData = true;
    _showExtraColumns = true;
    _dataDir = "data";

    //Database settings
    _databaseBackEnd = "MySQL";
    _databaseHost = "127.0.0.1";
    _databasePort = "3306";
    _databaseSchema = "MercurySteam";
    _databaseUsername = "root";
    _databasePassword = "";

    pt.put<bool>("Database.LoadInactiveData", _loadInactiveData);
    pt.put<bool>("Database.ShowExtraColumns", _showExtraColumns);
    pt.put<std::string>("General.DataDir", _dataDir);

    pt.put<std::string>("Database.Connection.Backend", _databaseBackEnd);
    pt.put<std::string>("Database.Connection.Host", _databaseHost);
    pt.put<std::string>("Database.Connection.Port", _databasePort);
    pt.put<std::string>("Database.Connection.Schema", _databaseSchema);
    pt.put<std::string>("Database.Connection.Username", _databaseUsername);
    pt.put<std::string>("Database.Connection.Password", _databasePassword);

    saveSettings(pt);
}

bool AppSettings::isLoadInactiveData() const
{
    return _loadInactiveData;
}

void AppSettings::setLoadInactiveData(bool value)
{
    _loadInactiveData = value;

    putSetting<bool>("Database.LoadInactiveData", value);
}

bool AppSettings::isShowExtraColumns() const
{
    return _showExtraColumns;
}

void AppSettings::setShowExtraColumns(bool value)
{
    _showExtraColumns = value;

    putSetting<bool>("Database.ShowExtraColumns", value);
}

std::string AppSettings::dataDir() const
{
    return _dataDir;
}

void AppSettings::setDataDir(const std::string &dataDir)
{
    _dataDir = dataDir;

    putSetting<std::string>("General.DataDir", dataDir);
}

std::string AppSettings::databaseBackEnd() const
{
    return _databaseBackEnd;
}

void AppSettings::setDatabaseBackEnd(const std::string &value)
{
    _databaseBackEnd = value;

    putSetting<std::string>("Database.Connection.BackEnd", value);
}

std::string AppSettings::databaseHost() const
{
    return _databaseHost;
}

void AppSettings::setDatabaseHost(const std::string &value)
{
    _databaseHost = value;

    putSetting<std::string>("Database.Connection.Host", value);
}

std::string AppSettings::databasePort() const
{
    return _databasePort;
}

void AppSettings::setDatabasePort(const std::string &value)
{
    _databasePort = value;

    putSetting<std::string>("Database.Connection.Port", value);
}

std::string AppSettings::databaseSchema() const
{
    return _databaseSchema;
}

void AppSettings::setDatabaseSchema(const std::string &value)
{
    _databaseSchema = value;

    putSetting<std::string>("Database.Connection.Schema", value);
}

std::string AppSettings::databaseUsername() const
{
    return _databaseUsername;
}

void AppSettings::setDatabaseUsername(const std::string &value)
{
    _databaseUsername = value;

    putSetting<std::string>("Database.Connection.UserName", value);
}

std::string AppSettings::databasePassword() const
{
    return _databasePassword;
}

void AppSettings::setDatabasePassword(const std::string &value)
{
    _databasePassword = value;

    putSetting<std::string>("Database.Connection.Password", value);
}

std::string AppSettings::appRoot() const
{
    return _appRoot;
}

void AppSettings::setAppRoot(const std::string &path)
{
    _appRoot = path;
}

std::string AppSettings::docRoot() const
{
    return _docRoot;
}

void AppSettings::setDocRoot(const std::string &path)
{
    _docRoot = path;
}
