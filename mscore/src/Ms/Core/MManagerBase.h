#ifndef MMANAGERBASE_H
#define MMANAGERBASE_H

#include "../Mscore_global.h"
#include "../Dbo/MDbo.h"
#include "../Log/MLogger.h"

#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>

namespace Ms
{
    namespace Core
    {
        class MSCORESHARED_EXPORT MManagerBase
        {

        public:
            MManagerBase();
            ~MManagerBase();

            std::string userName() const;
            void setUserName(const std::string &userName);

        protected:
            //variables
            std::string userName_;
            Ms::Log::MLogger _logger;

            //functions
            void _init();
        };
    }
}

#include "MManagerBase_impl.h"

#endif // MMANAGERBASE_H
