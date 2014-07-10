#ifndef MDBOQUERYMODEL_H
#define MDBOQUERYMODEL_H

#include <Wt/Dbo/QueryModel>
#include <Wt/Dbo/Query>
#include <Wt/WDateTime>
#include <Wt/WDate>
#include <Wt/WTime>
#include <Wt/WString>

#include <iostream>

#include <boost/any.hpp>
#include <boost/cast.hpp>

namespace Ms
{
    namespace Dbo
    {
        template <class Result>
        class MDboQueryModel : public Wt::Dbo::QueryModel<Result>
        {
        public:
            MDboQueryModel(Wt::WObject *parent = 0);

            //functions
            const std::string text(const Wt::WModelIndex &index, int role = Wt::DisplayRole) const;
            std::string userName();
            void setUserName(const std::string &userName);

            // WAbstractItemModel interface
            virtual boost::any data(const Wt::WModelIndex &index, int role) const;
            int getRowEditRank(int row) const;
            virtual bool setData(const Wt::WModelIndex &index, const boost::any &value, int role);

        private:
            //functions
            inline const std::string _boolToDBBool(bool b) const;
            inline const std::string _wDateToStdString(const Wt::WDate &date) const;
            inline const std::string _wTimeToStdString(const Wt::WTime &time) const;
            inline const std::string _wDateTimeToStdString(const Wt::WDateTime &dateTime) const;
            //variables
            std::string _userName;
        };
    }
}

#include "MDboQueryModel_impl.h"

#endif // MDBOQUERYMODEL_H
