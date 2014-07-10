#ifndef MDBOMANAGERBASE_H
#define MDBOMANAGERBASE_H

#include "../../Dbo/MDbo.h"
#include "../MManagerBase.h"

#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>

namespace Ms
{
    namespace Core
    {
        namespace Dbo
        {
            class MDboManagerBase : public Ms::Core::MManagerBase
            {

            public:
                MDboManagerBase();
                ~MDboManagerBase();

                Wt::Dbo::Session *session() const;
                void setSession(Wt::Dbo::Session *session);//doesn't take ownership of the session object
                bool openTransaction();
                bool commitTransaction();

                template<typename T>
                Wt::Dbo::ptr<T> createDbo(T *dbo);

                template<typename T>
                T* modifyDbo(Wt::Dbo::ptr<T> dboPtr);

            protected:
                //variables
                Wt::Dbo::Session *session_;
                Wt::Dbo::Transaction *transaction_;

                //functions

            };
        }
    }
}

#include "MDboManagerBase_impl.h"

#endif // MDBOMANAGERBASE_H
