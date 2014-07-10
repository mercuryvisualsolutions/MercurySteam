#ifndef MTABLEVIEWCOLUMN_H
#define MTABLEVIEWCOLUMN_H

#include <iostream>

#include <Wt/WFlags>
#include <Wt/WModelIndex>

#include "Delegates/MDelegates"

namespace Ms
{
    namespace Widgets
    {
        class MTableViewColumn
        {
        public:
            MTableViewColumn();

            MTableViewColumn(const std::string &dbFieldName);

            MTableViewColumn(const std::string &dbFieldName, const std::string &displayName);

            MTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags);

            MTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                            Ms::Widgets::Delegates::MItemDelegate *delegate);

            MTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                            Ms::Widgets::Delegates::MItemDelegate *delegate, bool isMandatory);

            MTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                            Ms::Widgets::Delegates::MItemDelegate *delegate, bool isMandatory, bool isIgnored);

            MTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
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
            bool operator ==(const Ms::Widgets::MTableViewColumn &other) const;


        private:
            std::string _dbFieldName;
            std::string _displayName;
            int _width;
            Wt::WFlags<Wt::ItemFlag> _flags;
            Ms::Widgets::Delegates::MItemDelegate *_delegate;
            bool _isMandatory;
            bool _ignored;
        };
    }
}

#endif // MTABLEVIEWCOLUMN_H
