#ifndef IO_H
#define IO_H

#include <iostream>
#include <vector>

#include "MFileInfo.h"
#include "Data/MDataCSV.h"

namespace Ms
{
    namespace IO
    {
        //directory separator
        inline char dirSeparator()
        {
            #ifdef _WIN32
                return '\\';
            #else
                return '/';
            #endif
        }

        //Returns the absolute path of the application's current directory.
        std::string currentPath();
        //Returns true if the dir path exists; otherwise returns false.
        bool dirExists(const std::string &path);
        //Returns true if the file exists; otherwise returns false.
        bool fileExists(const std::string &file);
        //Creates a sub-directory called dirName.
        bool createDir(const std::string &dirPath);
        //Creates the directory path dirPath.
        bool createPath(const std::string &dirPath);
        //Removes the directory, including all its contents.
        void removeRecursively(const std::string &dirPath);
        //Removes the contents of a directory.
        void removeChildDirs(const std::string &dirPath);
        //removes the given file
        bool removeFile(const std::string file);
        //copies the given file to the given path
        bool copyFile(const std::string file, const std::string path, bool overwrite = false);
        //Returns the absolute path
        std::string absolutePath(const std::string &path);
        //returns child dirs of the given root dir
        std::vector<std::string> dirChildren(const std::string &rootDir);
        //return files list in the given root directory
        std::vector<std::string> dirFiles(const std::string &rootDir);
        //return files list infor for the given root dir
        std::vector<Ms::IO::MFileInfo> dirFilesInfo(const std::string &rootDir);
        //returns children directories count for the given rootDir
        u_int64_t childDirCount(const std::string &rootDir);
        //returns files count inside the given rootDir
        u_int64_t dirFilesCount(const std::string &rootDir);
        //return temp directory path
        const std::string getTempPath();

        //files
        bool readCsv(const std::string &fileName, Ms::IO::Data::MDataCSV &dcsv);
        bool writeCsv(const std::string &fileName, Ms::IO::Data::MDataCSV &dcsv);
    }
}

#endif // IO_H
