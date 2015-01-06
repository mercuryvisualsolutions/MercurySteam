#ifndef MQUERYTABLEVIEWCOLUMN_H
#define MQUERYTABLEVIEWCOLUMN_H

#include <iostream>

#include <Wt/WFlags>
#include <Wt/WModelIndex>

#include "Delegates/MDelegates"

namespace Ms
{
    namespace Widgets
    {
        class MQueryTableViewColumn
        {
        public:
            MQueryTableViewColumn();

            MQueryTableViewColumn(const std::string &dbFieldName);

            MQueryTableViewColumn(const std::string &dbFieldName, const std::string &displayName);

            MQueryTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags);

            MQueryTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                            Ms::Widgets::Delegates::MItemDelegate *delegate);

            MQueryTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                            Ms::Widgets::Delegates::MItemDelegate *delegate, bool isMandatory);

            MQueryTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                            Ms::Widgets::Delegates::MItemDelegate *delegate, bool isMandatory, bool isIgnored);

            MQueryTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                            Ms::Widgets::Delegates::MItemDelegate *delegate, bool isMandatory, bool isIgnored, int width);

            //functions
            std::string dbFieldName() const;
            void setDbFieldName(const std::string &dbFieldName);
            std::string displayName() const;
            void setDisplayName(const std::string &displayName);
            int width() const;
            void setWidth(int width);
            Wt::WFlags<Wt::ItemFlag> flags() const;
            void setFlags(Wt::WFlags<Wt::ItemFlag> flags);
            Ms::Widgets::Delegates::MItemDelegate *delegate() const;
            void setDelegate(Delegates::MItemDelegate *delegate);
            bool isMandatory() const;
            void setMandatory(bool isMandatory);//whether the column is mandatory or not, used when importing CSV files
            bool isIgnored() const;//whether the column is ignored or not, used when importing CSV files
            void setIgnored(bool ignored);

            //operators
            bool operator ==(const Ms::Widgets::MQueryTableViewColumn &other) const;


        private:
            std::string m_dbFieldName;
            std::string m_displayName;
            int m_width;
            Wt::WFlags<Wt::ItemFlag> m_flags;
            Ms::Widgets::Delegates::MItemDelegate *m_delegate;
            bool m_isMandatory;
            bool m_ignored;
        };
    }
}

#endif // MQUERYTABLEVIEWCOLUMN_H
