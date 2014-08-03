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
    _extractExtension();
}

const std::string Ms::IO::MFileInfo::name() const
{
    return _name;
}

const std::string Ms::IO::MFileInfo::fullName() const
{
    return _fullName;
}

const std::string Ms::IO::MFileInfo::extension() const
{
    return _extension;
}

u_int64_t Ms::IO::MFileInfo::size() const
{
    return boost::filesystem::file_size(_fullName);
}

const std::string Ms::IO::MFileInfo::lastModifiedDate() const
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

void Ms::IO::MFileInfo::_extractExtension()
{
    _extension = _fullName.substr(_fullName.find_last_of('.') + 1, _fullName.length() -1);
}
