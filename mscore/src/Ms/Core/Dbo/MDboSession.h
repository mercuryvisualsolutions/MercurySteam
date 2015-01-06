#ifndef MDBOSESSION_H
#define MDBOSESSION_H

#include "../../Dbo/MDboBase.h"
#include "../MManagerBase.h"

#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>

namespace Ms
{
    namespace Core
    {
        namespace Dbo
        {
            class MDboSession : public Wt::Dbo::Session, public Ms::Core::MManagerBase
            {
            public:
                MDboSession(Wt::Dbo::SqlConnectionPool &connectionPool);
                ~MDboSession();

            public:
                template<typename T>
                Wt::Dbo::ptr<T> createDbo(T *dbo);

                template<typename T>
                T* modifyDbo(Wt::Dbo::ptr<T> dboPtr);

                template<typename T>
                std::vector<std::string> getDboIdFieldsNames() const;

                template<typename T>
                std::vector<std::string> getDboIdFieldsValues(const typename Wt::Dbo::dbo_traits<T>::IdType &id) const;

                template<typename T>
                Wt::Dbo::ptr<T> getDbo(const typename Wt::Dbo::dbo_traits<T>::IdType &id, bool forceReread = false);

                template<typename T>
                bool dboExists(const typename Wt::Dbo::dbo_traits<T>::IdType &id);

            protected:
                //variables
                Wt::Dbo::SqlConnectionPool &m_connectionPool;

                //functions

            };
        }
    }
}

#include "MDboSession_impl.h"

#endif // MDBOSESSION_H
