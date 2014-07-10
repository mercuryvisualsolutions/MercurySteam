#ifndef MSETTINGSBASE_H
#define MSETTINGSBASE_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace Ms
{
    namespace Core
    {
        class MSettingsBase
        {
        public:
            MSettingsBase();

            std::string settingsFileName();
            void setSettingsFileName(const std::string &fileName);
            void saveSettings(const std::string &fileName, const boost::property_tree::ptree &pt);
            void saveSettings(const boost::property_tree::ptree &pt);
            boost::property_tree::ptree loadSettings(const std::string &fileName);
            boost::property_tree::ptree loadSettings();

            template<typename T>
            T getSetting(const std::string &keyPath);

            template<typename T>
            T getSetting(const std::string &fileName, const std::string &keyPath);

            template<typename T>
            void putSetting(const std::string &keyPath, const T &value);

            template<typename T>
            void putSetting(const std::string &fileName, const std::string &keyPath, const T &value);

        private:
            //variables

            //functions
            void _saveSettings(const std::string &fileName, const boost::property_tree::ptree &pt);
            boost::property_tree::ptree _loadSettings(const std::string &fileName);

            template<typename T>
            T _getSetting(const std::string &fileName, const std::string &keyPath);

            template<typename T>
            void _putSetting(const std::string &fileName, const std::string &keyPath, const T &value);

        protected:
            std::string settingsFileName_;
        };
    }
}

#include "MSettingsBase_impl.h"

#endif // MSETTINGSBASE_H
