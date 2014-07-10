#include "MSettingsBase.h"

Ms::Core::MSettingsBase::MSettingsBase()
{
    settingsFileName_ = "Settings.xml";
}

std::string Ms::Core::MSettingsBase::settingsFileName()
{
    return settingsFileName_;
}

void Ms::Core::MSettingsBase::setSettingsFileName(const std::string &fileName)
{
    settingsFileName_ = fileName;
}

void Ms::Core::MSettingsBase::saveSettings(const std::string &fileName, const boost::property_tree::ptree &pt)
{
    _saveSettings(fileName, pt);
}

void Ms::Core::MSettingsBase::saveSettings(const boost::property_tree::ptree &pt)
{
    _saveSettings(settingsFileName_, pt);
}

boost::property_tree::ptree Ms::Core::MSettingsBase::loadSettings(const std::string &fileName)
{
    return _loadSettings(fileName);
}

boost::property_tree::ptree Ms::Core::MSettingsBase::loadSettings()
{
    return _loadSettings(settingsFileName_);
}

void Ms::Core::MSettingsBase::_saveSettings(const std::string &fileName, const boost::property_tree::ptree &pt)
{
    boost::property_tree::write_xml(fileName, pt);
}

boost::property_tree::ptree Ms::Core::MSettingsBase::_loadSettings(const std::string &fileName)
{
    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(fileName, pt);

    return pt;
}
