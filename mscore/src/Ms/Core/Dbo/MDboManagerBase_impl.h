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
        }
    }
}

#endif // MDBOMANAGERBASE_IMPL_H
