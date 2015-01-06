#ifndef MMANAGERBASE_H
#define MMANAGERBASE_H

#include "../Mscore_global.h"
#include "../Dbo/MDboBase.h"
#include "../Log/MLogger.h"

#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>

namespace Ms
{
    namespace Core
    {
        class SHARED_LIB_EXPORT MManagerBase
        {

        public:
            MManagerBase();
            ~MManagerBase();

            std::string userName() const;
            void setUserName(const std::string &userName) const;

        protected:
            //variables
            mutable std::string m_userName;
            Ms::Log::MLogger m_logger;

            //functions
            void init();
        };
    }
}

#endif // MMANAGERBASE_H
