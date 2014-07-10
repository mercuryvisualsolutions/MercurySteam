#include "MStreamedMemoryResource.h"

#include <Wt/Http/ResponseContinuation>
#include <Wt/Http/Response>

#include <fstream>

#include <boost/scoped_array.hpp>

Ms::IO::MStreamedMemoryResource::MStreamedMemoryResource(const std::vector<unsigned char> data, WObject *parent) :
    Wt::WMemoryResource(parent)
{
    isInUse_ = true;
    setData(data);
}

Ms::IO::MStreamedMemoryResource::~MStreamedMemoryResource()
{
    beingDeleted();
}

void Ms::IO::MStreamedMemoryResource::handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response)
{
    Wt::Http::ResponseContinuation *continuation = request.continuation();

    std::vector<unsigned char>::size_type dataSize = position + BUFFER_SIZE < data().size() ? BUFFER_SIZE : data().size() - position;

    response.out().write(reinterpret_cast<const char*>(&data()[0]) + position, dataSize);
    position += dataSize;

    if (position < data().size())
    {
        continuation= response.createContinuation();
        continuation->setData(data());
    }
    else
    {
        response.out().flush();

        isInUse_ = false;
    }
}
