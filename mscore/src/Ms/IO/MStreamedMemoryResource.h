#ifndef MSTREAMEDMEMORYRESOURCE_H
#define MSTREAMEDMEMORYRESOURCE_H

#include "MResource.h"

#include <Wt/WMemoryResource>

namespace Ms
{
    namespace IO
    {
        //A streamed memory resource that deletes itself -if required- after the download finishes
        class MStreamedMemoryResource :  public Ms::IO::MResource, public Wt::WMemoryResource
        {
            #define BUFFER_SIZE 4096

        public:
            MStreamedMemoryResource(const std::vector<unsigned char> data, WObject* parent = 0);
            ~MStreamedMemoryResource();

            // WResource interface
            void handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response) override;

        private:
            //variables
            std::vector<unsigned char>::size_type position = 0;
        };
    }
}

#endif // MSTREAMEDMEMORYRESOURCE_H
