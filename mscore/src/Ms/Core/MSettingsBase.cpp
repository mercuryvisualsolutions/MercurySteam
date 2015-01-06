#include "MSettingsBase.h"

Ms::Core::MSettingsBase::MSettingsBase()
{
    m_settingsFileName = "Settings.xml";
}

std::string Ms::Core::MSettingsBase::settingsFileName()
{
    return m_settingsFileName;
}

void Ms::Core::MSettingsBase::setSettingsFileName(const std::string &fileName)
{
    m_settingsFileName = fileName;
}

void Ms::Core::MSettingsBase::saveSettings(const std::string &fileName, const boost::property_tree::ptree &pt)
{
    boost::property_tree::write_xml(fileName, pt);
}

void Ms::Core::MSettingsBase::saveSettings(const boost::property_tree::ptree &pt)
{
    saveSettings(m_settingsFileName, pt);
}

boost::property_tree::ptree Ms::Core::MSettingsBase::loadSettings(const std::string &fileName)
{
    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(fileName, pt);

    return pt;
}

boost::property_tree::ptree Ms::Core::MSettingsBase::loadSettings()
{
    return loadSettings(m_settingsFileName);
}
