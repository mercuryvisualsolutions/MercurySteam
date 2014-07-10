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
            _userName = "SYSTEM";
        }

        //quick way to convert item data to string
        //TODO: do a better implementation
        template <class Result>
        const std::string Ms::Dbo::MDboQueryModel<Result>::text(const Wt::WModelIndex &index, int role) const
        {
            boost::any objData = index.data(role);

            if(objData.type() == typeid(std::string))
                return boost::any_cast<std::string>(objData);
            else if(objData.type() == typeid(int))
                return std::to_string(boost::any_cast<int>(objData));
            else if(objData.type() == typeid(unsigned int))
                return std::to_string(boost::any_cast<unsigned int>(objData));
            else if(objData.type() == typeid(long))
                return std::to_string(boost::any_cast<long>(objData));
            else if(objData.type() == typeid(unsigned long))
                return std::to_string(boost::any_cast<unsigned long>(objData));
            else if(objData.type() == typeid(long long))
                return std::to_string(boost::any_cast<long long>(objData));
            else if(objData.type() == typeid(unsigned long long))
                return std::to_string(boost::any_cast<unsigned long long>(objData));
            else if(objData.type() == typeid(float))
                return std::to_string(boost::any_cast<float>(objData));
            else if(objData.type() == typeid(double))
                return std::to_string(boost::any_cast<double>(objData));
            else if(objData.type() == typeid(long double))
                return std::to_string(boost::any_cast<long double>(objData));
            else if(objData.type() == typeid(bool))
                return _boolToDBBool(boost::any_cast<bool>(objData));
            else if(objData.type() == typeid(Wt::WDate))
                return _wDateToStdString(boost::any_cast<Wt::WDate>(objData));
            else if(objData.type() == typeid(Wt::WTime))
                return _wTimeToStdString(boost::any_cast<Wt::WTime>(objData));
            else if(objData.type() == typeid(Wt::WDateTime))
                return _wDateTimeToStdString(boost::any_cast<Wt::WDateTime>(objData));
            else if(objData.type() == typeid(Wt::WString))
                return boost::any_cast<Wt::WString>(objData).toUTF8();

            return "";
        }

        template <class Result>
        std::string Ms::Dbo::MDboQueryModel<Result>::userName()
        {
            return _userName;
        }

        template <class Result>
        void Ms::Dbo::MDboQueryModel<Result>::setUserName(const std::string &userName)
        {
            _userName = userName;
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
            Result &dboPtr = this->resultRow(index.row());

            dboPtr.modify()->modify()->lastModifiedDate_ = Wt::WDateTime::currentDateTime();
            dboPtr.modify()->modify()->lastModifiedBy_ = _userName;

            return Wt::Dbo::QueryModel<Result>::setData(index, value, role);
        }

        template <class Result>
        inline const std::string Ms::Dbo::MDboQueryModel<Result>::_boolToDBBool(bool b) const
        {
            return b ? "1" : "0";
        }

        template <class Result>
        inline const std::string Ms::Dbo::MDboQueryModel<Result>::_wDateToStdString(const Wt::WDate &date) const
        {
            return std::to_string(date.year()) + "-" + std::to_string(date.month()) + "-" + std::to_string(date.day());
        }

        template <class Result>
        inline const std::string Ms::Dbo::MDboQueryModel<Result>::_wTimeToStdString(const Wt::WTime &time) const
        {
            return std::to_string(time.hour()) + ":" + std::to_string(time.minute()) + ":" + std::to_string(time.second());
        }

        template <class Result>
        inline const std::string Ms::Dbo::MDboQueryModel<Result>::_wDateTimeToStdString(const Wt::WDateTime &dateTime) const
        {
            return _wDateToStdString(dateTime.date()) + " " + _wTimeToStdString(dateTime.time());
        }
    }
}

#endif // MDBOQUERYMODEL_IMPL_H
