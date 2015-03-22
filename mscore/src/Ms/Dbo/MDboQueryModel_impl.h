#ifndef MDBOQUERYMODEL_IMPL_H
#define MDBOQUERYMODEL_IMPL_H

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
        Ms::Dbo::MDboQueryModel<Result>::MDboQueryModel(Wt::WObject *parent) :
            Wt::Dbo::QueryModel<Result>(parent)
        {
            m_userName = "SYSTEM";
        }

        template <class Result>
        Ms::Dbo::MDboQueryModel<Result>::~MDboQueryModel()
        {
        }

        template <class Result>
        std::string Ms::Dbo::MDboQueryModel<Result>::userName()
        {
            return m_userName;
        }

        template <class Result>
        void Ms::Dbo::MDboQueryModel<Result>::setUserName(const std::string &userName)
        {
            m_userName = userName;
        }

        template<class Result>
        Wt::Signal<Wt::WModelIndex, boost::any, int> &Ms::Dbo::MDboQueryModel<Result>::dataAboutToBeChanged()
        {
            return m_dataAboutToBeChanged;
        }

        template <class Result>
        boost::any Ms::Dbo::MDboQueryModel<Result>::data(const Wt::WModelIndex &index, int role) const
        {
            if(role == Wt::DisplayRole || role == Wt::EditRole)
                return Wt::Dbo::QueryModel<Result>::data(index, role);
            else if(role == Wt::UserRole)//set userRole to edit rank for the whole row
                return getRowEditRank(index.row());

            return boost::any();
        }

        template <class Result>
        int Ms::Dbo::MDboQueryModel<Result>::getRowEditRank(int row) const
        {
            return this->resultRow(row)->editRank();
        }

        template <class Result>
        bool Ms::Dbo::MDboQueryModel<Result>::setData(const Wt::WModelIndex &index, const boost::any &value, int role)
        {
            m_dataAboutToBeChanged(index, value, role);

            Result &dboPtr = this->resultRow(index.row());

            dboPtr.modify()->modify()->m_lastModifiedDate = Wt::WDateTime::currentDateTime();
            dboPtr.modify()->modify()->m_lastModifiedBy = m_userName;

            return Wt::Dbo::QueryModel<Result>::setData(index, value, role);
        }
    }
}

#endif // MDBOQUERYMODEL_IMPL_H
