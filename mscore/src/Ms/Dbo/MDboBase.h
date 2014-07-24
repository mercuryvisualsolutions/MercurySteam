#ifndef MDBOBASE_H
#define MDBOBASE_H

#include <Wt/WDateTime>
#include <Wt/Dbo/Session>

namespace Ms
{
    namespace Core
    {
        namespace Dbo
        {
            class MDboManagerBase;
        }
    }

    namespace Dbo
    {
        template<class Result>
        class MDboQueryModel;

        class MDboBase
        {
            friend class Ms::Core::Dbo::MDboManagerBase;

            template<class Result>
            friend class Ms::Dbo::MDboQueryModel;

        public:
            MDboBase();
            MDboBase(bool active, int viewRank, int editRank, int removeRank);
            virtual ~MDboBase();

            virtual const std::string thumbnail() const;
            virtual void setThumbnail(const std::string &path);
            virtual const Wt::WDateTime dateCreated() const;
            virtual const Wt::WDateTime lastModifiedDate() const;
            virtual const std::string createdBy() const;
            virtual const std::string lastModifiedBy() const;
            virtual int viewRank() const;
            virtual void setViewRank(int rank);
            virtual int editRank() const;
            virtual void setEditRank(int rank);
            virtual int removeRank() const;
            virtual void setRemoveRank(int rank);
            virtual bool active() const;
            virtual void setActive(bool active);
            virtual MDboBase *modify();
            Ms::Core::Dbo::MDboManagerBase *dboManager() const;
            void setDboManager(Ms::Core::Dbo::MDboManagerBase *dboManager);

        protected:
            std::string thumbnail_;
            Wt::WDateTime dateCreated_;
            std::string createdBy_;
            Wt::WDateTime lastModifiedDate_;
            std::string lastModifiedBy_;
            bool active_;
            int viewRank_;
            int editRank_;
            int removeRank_;
            Ms::Core::Dbo::MDboManagerBase *dboManager_;

            //functions
            template<class Action>
            void persistDefaults_(Action &a);

        private:
            //functions
            void _init();

        };
    }
}

#include "MDboBase_impl.h"

#endif // MDBOBASE_H
