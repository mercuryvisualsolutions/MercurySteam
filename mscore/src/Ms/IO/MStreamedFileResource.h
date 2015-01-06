#ifndef MSTREAMEDFILERESOURCE_H
#define MSTREAMEDFILERESOURCE_H

#include "MResource.h"

#include <Wt/WFileResource>

namespace Ms
{
    namespace IO
    {
        //A streamed file resource that executes a system command and reads its output via a pipe
        class MStreamedFileResource : public Ms::IO::MResource, public Wt::WFileResource
        {
            #define BUFFER_SIZE 4096

        public:
            MStreamedFileResource(const std::string &fileName, bool deleteWhenFinished, WObject* parent = 0);
            ~MStreamedFileResource();

            //functions
            bool deleteWhenFinished();
            void setDeleteWhenFinished(bool del);

            // WResource interface
            void handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response) override;

        private:
            //variables
            char m_buffer[BUFFER_SIZE];
            bool m_deleteWhenFinished;

            //functions

        };
    }
}

#endif // MSTREAMEDFILERESOURCE_H
