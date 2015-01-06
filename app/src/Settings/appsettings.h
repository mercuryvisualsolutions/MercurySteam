#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <Ms/Core/MSettingsBase.h>

class AppSettings : public Ms::Core::MSettingsBase
{
private:
    AppSettings();
    AppSettings(const AppSettings &other);
    void operator=(const AppSettings &other);

public:
    static AppSettings &instance()
    {
        static AppSettings as;
        return as;
    }

    //functions
    void loadAppSettings();
    void initDefaultSettings();

    //General settings
    bool isLoadInactiveData() const;
    void setLoadInactiveData(bool value);
    bool isShowExtraColumns() const;
    void setShowExtraColumns(bool value);
    std::string dataDir() const;
    void setDataDir(const std::string &dataDir);

    //Database settings
    std::string databaseBackEnd() const;
    void setDatabaseBackEnd(const std::string &value);
    std::string databaseHost() const;
    void setDatabaseHost(const std::string &value);
    std::string databasePort() const;
    void setDatabasePort(const std::string &value);
    std::string databaseSchema() const;
    void setDatabaseSchema(const std::string &value);
    std::string databaseUsername() const;
    void setDatabaseUsername(const std::string &value);
    std::string databasePassword() const;
    void setDatabasePassword(const std::string &value);

    //Wt settings
    std::string appRoot() const;
    void setAppRoot(const std::string &path);
    std::string docRoot() const;
    void setDocRoot(const std::string &path);

private:
    //general
    bool m_loadInactiveData;
    bool m_showExtraColumns;
    std::string m_dataDir;

    //database
    std::string m_databaseBackEnd;
    std::string m_databaseHost;
    std::string m_databasePort;
    std::string m_databaseSchema;
    std::string m_databaseUsername;
    std::string m_databasePassword;

    //Wt
    std::string m_appRoot;
    std::string m_docRoot;
};

#endif // APPSETTINGS_H
