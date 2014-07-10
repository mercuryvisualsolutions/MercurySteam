#include "MFileInfo.h"

#define BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>

#include "IO.h"

Ms::IO::MFileInfo::MFileInfo()
{
}

Ms::IO::MFileInfo::MFileInfo(const std::string &fileName)
{
    _fullName = fileName;
    _extractName();
}

std::string Ms::IO::MFileInfo::name()
{
    return _name;
}

std::string Ms::IO::MFileInfo::fullName()
{
    return _fullName;
}

u_int64_t Ms::IO::MFileInfo::size()
{
    return boost::filesystem::file_size(_fullName);
}

std::string Ms::IO::MFileInfo::lastModifiedDate()
{
    time_t rawtime = boost::filesystem::last_write_time(_fullName);
    char buffer [80];

    strftime(buffer,80,"%Y-%m-%d %I:%M%p",localtime(&rawtime));
    std::string result(buffer);

    return result;
}

void Ms::IO::MFileInfo::_extractName()
{
    _name = _fullName.substr(_fullName.find_last_of(Ms::IO::dirSeparator()) + 1, _fullName.length() -1);
}
