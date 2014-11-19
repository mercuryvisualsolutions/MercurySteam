#include "MDboSession.h"
#include "../../Exceptions/MNullPointerException.h"

Ms::Core::Dbo::MDboSession::MDboSession(Wt::Dbo::SqlConnectionPool &connectionPool) :
    Ms::Core::MManagerBase(),
    _connectionPool(connectionPool)
{
    setConnectionPool(connectionPool);
}

Ms::Core::Dbo::MDboSession::~MDboSession()
{
}

