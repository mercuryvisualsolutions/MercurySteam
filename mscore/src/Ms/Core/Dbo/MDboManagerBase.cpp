#include "MDboManagerBase.h"
#include "../../Exceptions/MDboSessionIsNullException.h"

Ms::Core::Dbo::MDboManagerBase::MDboManagerBase() :
    Ms::Core::MManagerBase()
{
    session_ = nullptr;
    transaction_ = nullptr;
}

Ms::Core::Dbo::MDboManagerBase::~MDboManagerBase()
{
    delete transaction_;
}

Wt::Dbo::Session *Ms::Core::Dbo::MDboManagerBase::session() const
{
    return session_;
}

void Ms::Core::Dbo::MDboManagerBase::setSession(Wt::Dbo::Session *session)
{
    session_ = session;
}

bool Ms::Core::Dbo::MDboManagerBase::openTransaction()
{
    if(!session_)
        throw Ms::Exceptions::MDboSessionIsNullException("Can't create transaction object, DbSession object is null");

    if(!transaction_)
        transaction_ = new Wt::Dbo::Transaction(*session_);

    if(!transaction_->isActive())
    {
        delete transaction_;
        transaction_ = new Wt::Dbo::Transaction(*session_);
    }

    return true;
}

bool Ms::Core::Dbo::MDboManagerBase::commitTransaction()
{
    if(transaction_ && transaction_->isActive())
    {
        transaction_->commit();
        session_->flush();

        return true;
    }

    return false;
}
