#include "projectsio.h"
#include "../Settings/appsettings.h"

#include <Wt/Dbo/Dbo>

#include <Ms/IO/IO.h>

Projects::ProjectsIO::ProjectsIO()
{
}

bool Projects::ProjectsIO::createProjectDirectoryStructure(const std::string &prjName)
{
    bool result;
    std::string path =
            AppSettings::instance().docRoot() + Ms::IO::dirSeparator() +
            AppSettings::instance().dataDir() + Ms::IO::dirSeparator() + "projects" +
            Ms::IO::dirSeparator() + prjName;

    path = Ms::IO::absolutePath(path);

    result =  Ms::IO::createPath(path);
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "thumbnails");
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "sequences");
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "assets");
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "files");

    return result;
}

std::string Projects::ProjectsIO::getAbsoluteProjectDir(const std::string &prjName)
{
    std::string dir =  Ms::IO::absolutePath(AppSettings::instance().docRoot() + Ms::IO::dirSeparator() +
            AppSettings::instance().dataDir() + Ms::IO::dirSeparator() + "projects" +
                                              Ms::IO::dirSeparator() + prjName);

    if(!Ms::IO::dirExists(dir))
        createProjectDirectoryStructure(prjName);

    return dir;
}

std::string Projects::ProjectsIO::getRelativeProjectDir(const std::string &prjName)
{
    getAbsoluteProjectDir(prjName);//create the dir if it doesn't exist
    return AppSettings::instance().dataDir() + Ms::IO::dirSeparator() + "projects" + Ms::IO::dirSeparator() + prjName;
}

bool Projects::ProjectsIO::createSequenceDirectoryStructure(const std::string &prjName, const std::string &seqName)
{
    bool result;
    std::string path = getAbsoluteProjectDir(prjName) + Ms::IO::dirSeparator() + "sequences" + Ms::IO::dirSeparator() + seqName;
    result =  Ms::IO::createPath(path);
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "thumbnails");
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "files");

    return result;
}

std::string Projects::ProjectsIO::getAbsoluteSequenceDir(const std::string &prjName, const std::string &seqName)
{
    std::string dir = getAbsoluteProjectDir(prjName) + Ms::IO::dirSeparator() + "sequences" + Ms::IO::dirSeparator() + seqName;

    if(!Ms::IO::dirExists(dir))
        createSequenceDirectoryStructure(prjName, seqName);

    return dir;
}

std::string Projects::ProjectsIO::getRelativeSequenceDir(const std::string &prjName, const std::string &seqName)
{
    getAbsoluteSequenceDir(prjName, seqName);//create the dir if it doesn't exist
    return getRelativeProjectDir(prjName) + Ms::IO::dirSeparator() + "sequences" + Ms::IO::dirSeparator() + seqName;
}

bool Projects::ProjectsIO::createShotDirectoryStructure(const std::string &prjName, const std::string &seqName, const std::string &shotName)
{
    bool result;
    std::string path = getAbsoluteSequenceDir(prjName, seqName) + Ms::IO::dirSeparator() + "shots" + Ms::IO::dirSeparator() + shotName;
    result =  Ms::IO::createPath(path);
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "thumbnails");
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "files");

    return result;
}

std::string Projects::ProjectsIO::getAbsoluteShotDir(const std::string &prjName, const std::string &seqName, const std::string &shotName)
{
    std::string dir = getAbsoluteSequenceDir(prjName, seqName) + Ms::IO::dirSeparator() + "shots" + Ms::IO::dirSeparator() + shotName;

    if(!Ms::IO::dirExists(dir))
        createShotDirectoryStructure(prjName, seqName, shotName);

    return dir;
}

std::string Projects::ProjectsIO::getRelativeShotDir(const std::string &prjName, const std::string &seqName, const std::string &shotName)
{
    getAbsoluteShotDir(prjName, seqName, shotName);
    return getRelativeSequenceDir(prjName, seqName) + Ms::IO::dirSeparator() + "shots" + Ms::IO::dirSeparator() + shotName;
}

bool Projects::ProjectsIO::createAssetDirectoryStructure(const std::string &prjName, const std::string &assetName)
{
    bool result;
    std::string path = getAbsoluteProjectDir(prjName) + Ms::IO::dirSeparator() + "assets" + Ms::IO::dirSeparator() + assetName;
    result =  Ms::IO::createPath(path);
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "thumbnails");
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "files");

    return result;
}

std::string Projects::ProjectsIO::getAbsoluteAssetDir(const std::string &prjName, const std::string &assetName)
{
    std::string dir = getAbsoluteProjectDir(prjName) + Ms::IO::dirSeparator() + "assets" + Ms::IO::dirSeparator() + assetName;

    if(!Ms::IO::dirExists(dir))
        createAssetDirectoryStructure(prjName, assetName);

    return dir;
}

std::string Projects::ProjectsIO::getRelativeAssetDir(const std::string &prjName, const std::string &assetName)
{
    getAbsoluteAssetDir(prjName, assetName);
    return getRelativeProjectDir(prjName) + Ms::IO::dirSeparator() + "assets" + Ms::IO::dirSeparator() + assetName;
}

bool Projects::ProjectsIO::createProjectTaskDirectoryStructure(const std::string &prjName, u_int64_t taskId)
{
    bool result;
    std::string path = getAbsoluteProjectDir(prjName) + Ms::IO::dirSeparator() + "tasks" + Ms::IO::dirSeparator() + std::to_string(taskId);
    result =  Ms::IO::createPath(path);
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "thumbnails");
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "files");

    return result;
}

std::string Projects::ProjectsIO::getAbsoluteProjectTaskDir(const std::string &prjName, u_int64_t taskId)
{
    std::string dir = getAbsoluteProjectDir(prjName) + Ms::IO::dirSeparator() + "tasks" + Ms::IO::dirSeparator() + std::to_string(taskId);

    if(!Ms::IO::dirExists(dir))
        createProjectTaskDirectoryStructure(prjName, taskId);

    return dir;
}

std::string Projects::ProjectsIO::getRelativeProjectTaskDir(const std::string &prjName, u_int64_t taskId)
{
    getAbsoluteProjectTaskDir(prjName, taskId);
    return getRelativeProjectDir(prjName) + Ms::IO::dirSeparator() + "tasks" + Ms::IO::dirSeparator() + std::to_string(taskId);
}

bool Projects::ProjectsIO::createSequenceTaskDirectoryStructure(const std::string &prjName, const std::string &sequenceName, u_int64_t taskId)
{
    bool result;
    std::string path = getAbsoluteSequenceDir(prjName, sequenceName) + Ms::IO::dirSeparator() + "tasks" + Ms::IO::dirSeparator() + std::to_string(taskId);
    result =  Ms::IO::createPath(path);
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "thumbnails");
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "files");

    return result;
}

std::string Projects::ProjectsIO::getAbsoluteSequenceTaskDir(const std::string &prjName, const std::string &sequenceName, u_int64_t taskId)
{
    std::string dir = getAbsoluteSequenceDir(prjName, sequenceName) + Ms::IO::dirSeparator() + "tasks" + Ms::IO::dirSeparator() + std::to_string(taskId);

    if(!Ms::IO::dirExists(dir))
        createSequenceTaskDirectoryStructure(prjName, sequenceName, taskId);

    return dir;
}

std::string Projects::ProjectsIO::getRelativeSequenceTaskDir(const std::string &prjName, const std::string &sequenceName, u_int64_t taskId)
{
    getAbsoluteSequenceTaskDir(prjName, sequenceName, taskId);
    return getRelativeSequenceDir(prjName, sequenceName) + Ms::IO::dirSeparator() + "tasks" + Ms::IO::dirSeparator() + std::to_string(taskId);
}

bool Projects::ProjectsIO::createAssetTaskDirectoryStructure(const std::string &prjName, const std::string &assetName, u_int64_t taskId)
{
    bool result;
    std::string path = getAbsoluteAssetDir(prjName, assetName) + Ms::IO::dirSeparator() + "tasks" + Ms::IO::dirSeparator() + std::to_string(taskId);
    result =  Ms::IO::createPath(path);
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "thumbnails");
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "files");

    return result;
}

std::string Projects::ProjectsIO::getAbsoluteAssetTaskDir(const std::string &prjName, const std::string &assetName, u_int64_t taskId)
{
    std::string dir = getAbsoluteAssetDir(prjName, assetName) + Ms::IO::dirSeparator() + "tasks" + Ms::IO::dirSeparator() + std::to_string(taskId);

    if(!Ms::IO::dirExists(dir))
        createAssetTaskDirectoryStructure(prjName, assetName, taskId);

    return dir;
}

std::string Projects::ProjectsIO::getRelativeAssetTaskDir(const std::string &prjName, const std::string &assetName, u_int64_t taskId)
{
    getAbsoluteAssetTaskDir(prjName, assetName, taskId);
    return getRelativeAssetDir(prjName, assetName) + Ms::IO::dirSeparator() + "tasks" + Ms::IO::dirSeparator() + std::to_string(taskId);
}

bool Projects::ProjectsIO::createShotTaskDirectoryStructure(const std::string &prjName, const std::string &seqName, const std::string &shotName, u_int64_t taskId)
{
    bool result;
    std::string path = getAbsoluteShotDir(prjName, seqName, shotName) + Ms::IO::dirSeparator() + "tasks" + Ms::IO::dirSeparator() + std::to_string(taskId);
    result =  Ms::IO::createPath(path);
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "thumbnails");
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "files");

    return result;
}

std::string Projects::ProjectsIO::getAbsoluteShotTaskDir(const std::string &prjName, const std::string &seqName, const std::string &shotName, u_int64_t taskId)
{
    std::string dir = getAbsoluteShotDir(prjName, seqName, shotName) + Ms::IO::dirSeparator() + "tasks" + Ms::IO::dirSeparator() + std::to_string(taskId);

    if(!Ms::IO::dirExists(dir))
        createShotTaskDirectoryStructure(prjName, seqName, shotName, taskId);

    return dir;
}

std::string Projects::ProjectsIO::getRelativeShotTaskDir(const std::string &prjName, const std::string &seqName, const std::string &shotName, u_int64_t taskId)
{
    getAbsoluteShotTaskDir(prjName, seqName, shotName, taskId);
    return getRelativeShotDir(prjName, seqName, shotName) + Ms::IO::dirSeparator() + "tasks" + Ms::IO::dirSeparator() + std::to_string(taskId);
}
