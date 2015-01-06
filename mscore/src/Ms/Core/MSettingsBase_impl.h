#ifndef MSETTINGSBASE_IMPL_H
#define MSETTINGSBASE_IMPL_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace Ms
{
    namespace Core
    {
        template<typename T>
        T Ms::Core::MSettingsBase::getSetting(const std::string &keyPath)
        {
            return getSetting<T>(m_settingsFileName, keyPath);
        }

        template<typename T>
        T Ms::Core::MSettingsBase::getSetting(const std::string &fileName, const std::string &keyPath)
        {
            boost::property_tree::ptree pt;

            boost::property_tree::read_xml(fileName, pt);

            return pt.get<T>(keyPath);
        }

        template<typename T>
        void Ms::Core::MSettingsBase::putSetting(const std::string &keyPath, const T &value)
        {
            putSetting(m_settingsFileName, keyPath, value);
        }

        template<typename T>
        void Ms::Core::MSettingsBase::putSetting(const std::string &fileName, const std::string &keyPath, const T &value)
        {
            boost::property_tree::ptree pt;
            boost::property_tree::read_xml(fileName, pt);

            pt.put<T>(keyPath, value);

            boost::property_tree::write_xml(fileName, pt);
        }
    }
}

#endif // MSETTINGSBASE_IMPL_H
