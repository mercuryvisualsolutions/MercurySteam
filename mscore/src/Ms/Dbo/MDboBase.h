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
            class MDboSession;
        }
    }

    namespace Dbo
    {
        template<class Result>
        class MDboQueryModel;

        class MDboBase
        {
            friend class Ms::Core::Dbo::MDboSession;

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
            const Core::Dbo::MDboSession *dboSession() const;
            void setDboManager(const Ms::Core::Dbo::MDboSession *dboSession);

        protected:
            std::string m_thumbnail;
            Wt::WDateTime m_dateCreated;
            std::string m_createdBy;
            Wt::WDateTime m_lastModifiedDate;
            std::string m_lastModifiedBy;
            bool m_active;
            int m_viewRank;
            int m_editRank;
            int m_removeRank;
            const Ms::Core::Dbo::MDboSession *m_dboSession;

            //functions
            template<class Action>
            void persist(Action &a);

        private:
            //functions
            void init();

        };
    }
}

#include "MDboBase_impl.h"

#endif // MDBOBASE_H
