#ifndef MDBOSESSION_IMPL_H
#define MDBOSESSION_IMPL_H

#include <Wt/Dbo/ptr>
#include <Wt/Dbo/Transaction>

namespace Ms
{
    namespace Core
    {
        namespace Dbo
        {
            //Requires an active transaction
            template<typename T>
            Wt::Dbo::ptr<T> Ms::Core::Dbo::MDboSession::createDbo(T *dbo)
            {
                Wt::Dbo::ptr<T> dboPtr;

                if(dbo)
                {
                    dbo->m_createdBy = m_userName;
                    dbo->m_dateCreated = Wt::WDateTime::currentDateTime();

                    try
                    {
                        dboPtr = add(dbo);
                    }
                    catch(Wt::Dbo::Exception ex)
                    {
                        std::cout << "Error occured while trying to add new Dbo to database" << std::endl << ex.what() << std::endl;
                    }
                }

                return dboPtr;
            }

            //Requires an active transaction
            template<typename T>
            T* Ms::Core::Dbo::MDboSession::modifyDbo(Wt::Dbo::ptr<T> dboPtr)
            {
                if(dboPtr)
                {
                    dboPtr.modify()->modify()->m_lastModifiedDate = Wt::WDateTime::currentDateTime();
                    dboPtr.modify()->modify()->m_lastModifiedBy = m_userName;
                }

                return dboPtr.modify()->modify();
            }

            template<typename T>
            std::vector<std::string> Ms::Core::Dbo::MDboSession::getDboIdFieldsNames() const
            {
                std::vector<std::string> fields;

                std::stringstream ssFields;
                ssFields << Wt::Dbo::dbo_traits<T>::surrogateIdField();

                std::string strTmp = "";

                //extract fields name
                while(std::getline(ssFields, strTmp, ','))
                    fields.push_back(strTmp);

                return fields;
            }

            template<typename T>
            std::vector<std::string> Ms::Core::Dbo::MDboSession::getDboIdFieldsValues(const typename Wt::Dbo::dbo_traits<T>::IdType &id) const
            {
                std::vector<std::string> values;

                std::stringstream ssValues;
                ssValues << id;

                std::string strTmp = "";

                //extract fields name
                while(std::getline(ssValues, strTmp, ','))
                    values.push_back(strTmp);

                return values;
            }

            template<typename T>
            Wt::Dbo::ptr<T> Ms::Core::Dbo::MDboSession::getDbo(const typename Wt::Dbo::dbo_traits<T>::IdType &id, bool forceReread)
            {
                Wt::Dbo::ptr<T> dboPtr;

                try
                {
                    dboPtr = load<T>(id, forceReread);
                }
                catch(Wt::Dbo::ObjectNotFoundException)
                {
                    //do nothing
                }
                catch(Wt::Dbo::Exception ex)
                {
                    std::cout << "Error occured while trying to get Dbo from database" << std::endl << ex.what() << std::endl;
                }


                return dboPtr;
            }

            template<typename T>
            bool Ms::Core::Dbo::MDboSession::dboExists(const typename Wt::Dbo::dbo_traits<T>::IdType &id)
            {
                return getDbo<T>(id).get() != nullptr;
            }
        }
    }
}

#endif // MDBOSESSION_IMPL_H
