#ifndef PROJECTSIO_H
#define PROJECTSIO_H

#include <iostream>

namespace Projects
{
    class ProjectsIO
    {
    public:
        ProjectsIO();

        //functions
        //creates the default directory structure for project prjName
        static bool createProjectDirectoryStructure(const std::string &prjName);
        //return the absolute path for a project prjName, but never checks if directory exist or not
        static std::string getAbsoluteProjectDir(const std::string &prjName);
        //return the path for a project prjName relative to current directory, but never checks if directory exist or not
        static std::string getRelativeProjectDir(const std::string &prjName);

        //creates the default directory structure for sequence seqName for project prjName
        static bool createSequenceDirectoryStructure(const std::string &prjName, const std::string &seqName);
        //return the absolute path for the sequence directory, but never checks if directory exist or not
        static std::string getAbsoluteSequenceDir(const std::string &prjName, const std::string &seqName);
        //return the path for for the sequence directory relative to current directory, but never checks if directory exist or not
        static std::string getRelativeSequenceDir(const std::string &prjName, const std::string &seqName);

        //creates the default directory structure for shot shotName for sequence seqName for project prjName
        static bool createShotDirectoryStructure(const std::string &prjName, const std::string &seqName, const std::string &shotName);
        //return the absolute path for the shot directory, but never checks if directory exist or not
        static std::string getAbsoluteShotDir(const std::string &prjName, const std::string &seqName, const std::string &shotName);
        //return the path for for the shot directory relative to current directory, but never checks if directory exist or not
        static std::string getRelativeShotDir(const std::string &prjName, const std::string &seqName, const std::string &shotName);

        //creates the default directory structure for sequence seqName for asset assetName
        static bool createAssetDirectoryStructure(const std::string &prjName, const std::string &assetName);
        //return the absolute path for the assets directory, but never checks if directory exist or not
        static std::string getAbsoluteAssetDir(const std::string &prjName, const std::string &assetName);
        //return the path for for the assets directory relative to current directory, but never checks if directory exist or not
        static std::string getRelativeAssetDir(const std::string &prjName, const std::string &assetName);

        //creates the default directory structure for task shotName
        static bool createAssetTaskDirectoryStructure(const std::string &prjName, const std::string &assetName, u_int64_t taskId);
        static bool createShotTaskDirectoryStructure(const std::string &prjName, const std::string &seqName, const std::string &shotName, u_int64_t taskId);
        //return the absolute path for the assets directory, but never checks if directory exist or not
        static std::string getAbsoluteAssetTaskDir(const std::string &prjName, const std::string &assetName, u_int64_t taskId);
        static std::string getAbsoluteShotTaskDir(const std::string &prjName, const std::string &seqName, const std::string &shotName, u_int64_t taskId);
        //return the path for for the assets directory relative to current directory, but never checks if directory exist or not
        static std::string getRelativeAssetTaskDir(const std::string &prjName, const std::string &assetName, u_int64_t taskId);
        static std::string getRelativeShotTaskDir(const std::string &prjName, const std::string &seqName, const std::string &shotName, u_int64_t taskId);
    };
}

#endif // PROJECTSIO_H
