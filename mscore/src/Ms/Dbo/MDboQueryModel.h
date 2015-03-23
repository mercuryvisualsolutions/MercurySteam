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
            ~MDboQueryModel();

            //functions
            std::string userName();
            void setUserName(const std::string &userName);

            //signals
            Wt::Signal<const Wt::WModelIndex, const boost::any, int> &dataAboutToBeChanged();

            // WAbstractItemModel interface
            virtual boost::any data(const Wt::WModelIndex &index, int role) const;
            int getRowEditRank(int row) const;
            virtual bool setData(const Wt::WModelIndex &index, const boost::any &value, int role);

        private:
            //functions

            //signal
            Wt::Signal<const Wt::WModelIndex,const boost::any,int> m_dataAboutToBeChanged;

            //variables
            std::string m_userName;
        };
    }
}

#include "MDboQueryModel_impl.h"

#endif // MDBOQUERYMODEL_H
