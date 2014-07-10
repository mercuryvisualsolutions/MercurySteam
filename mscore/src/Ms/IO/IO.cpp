#include "IO.h"

#define BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>

std::string Ms::IO::currentPath()
{
    return boost::filesystem::current_path().string();
}

bool Ms::IO::dirExists(const std::string &path)
{
    if(boost::filesystem::exists(path))
        return boost::filesystem::is_directory(path);

    return false;
}

bool Ms::IO::fileExists(const std::string &file)
{
    if(boost::filesystem::exists(file))
        return boost::filesystem::is_regular_file(file);

    return false;
}

bool Ms::IO::createDir(const std::string &dirPath)
{
    return boost::filesystem::create_directory(dirPath);
}

bool Ms::IO::createPath(const std::string &dirPath)
{
    return boost::filesystem::create_directories(dirPath);
}

void Ms::IO::removeRecursively(const std::string &dirPath)
{
    boost::filesystem::remove_all(dirPath);
}

void Ms::IO::removeChildDirs(const std::string &dirPath)
{
    boost::filesystem::path path(dirPath);

    for(boost::filesystem::directory_iterator end_dir_iter, iter(path); iter !=end_dir_iter; ++iter)
    {
        boost::filesystem::remove_all(iter->path());
    }
}

bool Ms::IO::removeFile(const std::string file)
{
    if(fileExists(file))
        return boost::filesystem::remove(file);

    return false;
}

bool Ms::IO::copyFile(const std::string file, const std::string path, bool overwrite)
{
    if(fileExists(file))
    {
        boost::filesystem::copy_file(
                    file, path, overwrite ? boost::filesystem::copy_option::overwrite_if_exists : boost::filesystem::copy_option::fail_if_exists);

        return true;
    }

    return false;
}

std::string Ms::IO::absolutePath(const std::string &path)
{
    return boost::filesystem::absolute(path).string();
}

std::vector<std::string> Ms::IO::dirChildren(const std::string &rootDir)
{
    boost::filesystem::path path(rootDir);
    std::vector<std::string> dirs;

    for(boost::filesystem::directory_iterator end_dir_iter, iter(path); iter !=end_dir_iter; ++iter)
    {
        if(boost::filesystem::is_directory(iter->status()))
            dirs.push_back(iter->path().filename().string());
    }

    return dirs;
}

std::vector<std::string> Ms::IO::dirFiles(const std::string &rootDir)
{
    boost::filesystem::path path(rootDir);
    std::vector<std::string> files;

    for(boost::filesystem::directory_iterator end_dir_iter, iter(path); iter !=end_dir_iter; ++iter)
    {
        if(boost::filesystem::is_regular_file(iter->status()))
            files.push_back(iter->path().string());
    }

    return files;
}

std::vector<Ms::IO::MFileInfo> Ms::IO::dirFilesInfo(const std::string &rootDir)
{
    std::vector<std::string> files = Ms::IO::dirFiles(rootDir);
    std::vector<Ms::IO::MFileInfo> filesInfo;

    for(auto file : files)
    {
        Ms::IO::MFileInfo fileInfo(file);
        filesInfo.push_back(fileInfo);
    }

    return filesInfo;
}


u_int64_t Ms::IO::childDirCount(const std::string &rootDir)
{
    return Ms::IO::dirChildren(rootDir).size();
}

const std::string Ms::IO::getTempPath()
{
    return boost::filesystem::temp_directory_path().string();
}


u_int64_t Ms::IO::dirFilesCount(const std::string &rootDir)
{
    return Ms::IO::dirFiles(rootDir).size();
}


bool Ms::IO::readCsv(const std::string &fileName, Ms::IO::Data::MDataCSV &dcsv)
{
    if(!boost::filesystem::exists(fileName))
        return false;

    std::ifstream fileStream(fileName);
    if(!fileStream.is_open())
        return false;

    std::string line;

    while(std::getline(fileStream, line))
    {
        Ms::IO::Data::MDataCSVRow row(line);
        dcsv.addRow(row);
    }

    fileStream.close();

    return true;
}


bool Ms::IO::writeCsv(const std::string &fileName, Ms::IO::Data::MDataCSV &dcsv)
{
    if(!boost::filesystem::exists(fileName))
        return false;

    std::ofstream fileStream(fileName);
    if(!fileStream.is_open())
        return false;

    for(std::size_t i = 0; i < dcsv.numRows(); ++i)
    {
        fileStream << dcsv[i].toString();

        if(i < dcsv.numRows() -1)
            fileStream << "\n";
    }

    fileStream.close();

    return true;
}
