#ifndef MDBOMANAGERBASE_IMPL_H
#define MDBOMANAGERBASE_IMPL_H

#include <Wt/Dbo/ptr>

namespace Ms
{
    namespace Core
    {
        namespace Dbo
        {
            template<typename T>
            Wt::Dbo::ptr<T> Ms::Core::Dbo::MDboManagerBase::createDbo(T *dbo)
            {
                Wt::Dbo::ptr<T> dboPtr;

                if(openTransaction() && dbo)
                {
                    dbo->createdBy_ = userName_;
                    dbo->dateCreated_ = Wt::WDateTime::currentDateTime();

                    try
                    {
                        dboPtr = session_->add(dbo);

                        commitTransaction();
                    }
                    catch(Wt::Dbo::Exception ex)
                    {
                        std::cout << "Error occured while trying to add new Dbo to database" << std::endl << ex.what() << std::endl;
                    }
                }

                return dboPtr;
            }

            template<typename T>
            T* Ms::Core::Dbo::MDboManagerBase::modifyDbo(Wt::Dbo::ptr<T> dboPtr)
            {
                if(openTransaction() && dboPtr)
                {
                    dboPtr.modify()->modify()->lastModifiedDate_ = Wt::WDateTime::currentDateTime();
                    dboPtr.modify()->modify()->lastModifiedBy_ = userName_;

                    commitTransaction();
                }

                return dboPtr.modify()->modify();
            }

            template<typename T>
            std::vector<std::string> Ms::Core::Dbo::MDboManagerBase::getDboIdFieldsNames()
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
            std::vector<std::string> Ms::Core::Dbo::MDboManagerBase::getDboIdFieldsValues(const typename Wt::Dbo::dbo_traits<T>::IdType &id)
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
            Wt::Dbo::ptr<T> Ms::Core::Dbo::MDboManagerBase::getDbo(const typename Wt::Dbo::dbo_traits<T>::IdType &id, bool forceReread)
            {
                Wt::Dbo::ptr<T> dboPtr;

                if(openTransaction())
                {
                    try
                    {
                        dboPtr = session_->load<T>(id, forceReread);

                        commitTransaction();
                    }
                    catch(Wt::Dbo::ObjectNotFoundException)
                    {
                        //do nothing
                    }
                    catch(Wt::Dbo::Exception ex)
                    {
                        std::cout << "Error occured while trying to get Dbo from database" << std::endl << ex.what() << std::endl;
                    }
                }

                return dboPtr;
            }

            template<typename T>
            bool Ms::Core::Dbo::MDboManagerBase::dboExists(const typename Wt::Dbo::dbo_traits<T>::IdType &id)
            {
                return getDbo<T>(id).get() != nullptr;
            }
        }
    }
}

#endif // MDBOMANAGERBASE_IMPL_H
