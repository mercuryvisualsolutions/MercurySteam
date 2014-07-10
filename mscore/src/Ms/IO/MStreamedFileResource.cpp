#include "MStreamedFileResource.h"

#include <Wt/Http/ResponseContinuation>
#include <Wt/Http/Response>

#include <fstream>

#include <boost/scoped_array.hpp>

Ms::IO::MStreamedFileResource::MStreamedFileResource(const std::string &fileName, bool deleteWhenFinished, WObject *parent) :
    Wt::WFileResource(parent),
    _deleteWhenFinished(deleteWhenFinished)
{
    setFileName(fileName);
}

Ms::IO::MStreamedFileResource::~MStreamedFileResource()
{
    beingDeleted();
}

bool Ms::IO::MStreamedFileResource::deleteWhenFinished()
{
    return _deleteWhenFinished;
}

void Ms::IO::MStreamedFileResource::setDeleteWhenFinished(bool del)
{
    _deleteWhenFinished = del;
}

void Ms::IO::MStreamedFileResource::handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response)
{
    Wt::Http::ResponseContinuation *continuation = request.continuation();
    FILE *src = continuation ? boost::any_cast<FILE *>(continuation->data()) : 0;

    if (!src)//Initial request (not a continuation)
    {
        isInUse_ = true;

        src = fopen(fileName().c_str(), "r");

        if (!src)
        {
            response.setStatus(404);
            return;
        }

        //response.setMimeType("application/octet-stream");
    }

    size_t actualSize = fread(_buffer, 1, BUFFER_SIZE, src);
    response.out().write(_buffer, actualSize);

    if (!feof(src))
    {
        continuation= response.createContinuation();
        continuation->setData(src);
    }
    else
    {
        response.out().flush();
        fclose(src);

        if(_deleteWhenFinished)
        {
            remove(fileName().c_str());
        }

        isInUse_ = false;
    }
}
