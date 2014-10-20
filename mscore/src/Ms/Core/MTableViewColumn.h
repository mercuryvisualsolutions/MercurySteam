#ifndef MTABLEVIEWCOLUMN_H
#define MTABLEVIEWCOLUMN_H

#include "../Widgets/Delegates/MDelegates"

#include <Wt/WFlags>
#include <Wt/WModelIndex>

#include <iostream>

namespace Ms
{
    namespace Core
    {
        class MTableViewColumn
        {
        public:
            MTableViewColumn();
            MTableViewColumn(const std::string &name, int width = 150, Wt::WFlags<Wt::ItemFlag> flags = Wt::ItemIsSelectable, bool isMandatory = false, bool isIgnored = false);

            std::string name() const;
            void setName(std::string &name);
            int width() const;
            void setWidth(int width);
            Wt::WFlags<Wt::ItemFlag> flags() const;
            void setFlags(Wt::WFlags<Wt::ItemFlag> flags);
            Ms::Widgets::Delegates::MItemDelegate *delegate() const;
            void setDelegate(Widgets::Delegates::MItemDelegate *delegate);
            bool isMandatory() const;
            void setMandatory(bool isMandatory);//whether the column is mandatory or not, used when importing CSV files
            bool isIgnored() const;//whether the column is ignored or not, used when importing CSV files
            void setIgnored(bool isIgnored);

            bool operator=(const Ms::Core::MTableViewColumn &other) const;


        protected:
            std::string _name;
            int _width;
            Wt::WFlags<Wt::ItemFlag> _flags;
            bool _isMandatory;
            bool _isIgnored;
            Ms::Widgets::Delegates::MItemDelegate *_delegate;
        };
    }
}

#endif // MTABLEVIEWCOLUMN_H
