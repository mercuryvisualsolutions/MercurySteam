#ifndef DLGFILESMANAGER_H
#define DLGFILESMANAGER_H

#include "../../Log/logger.h"

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WContainerWidget>
#include <Wt/WTree>
#include <Wt/WTreeNode>
#include <Wt/WTableView>
#include <Wt/WStandardItemModel>
#include <Wt/WToolBar>
#include <Wt/WFileResource>

#include <Ms/Widgets/MWidgetFactory.h>
#include <Ms/IO/MStreamedFileResource.h>

namespace Views
{
    class DlgFilesManager : public Wt::WDialog
    {
    public:
        DlgFilesManager(const std::string &rootPath);

        //functions
        void setRootpath(const std::string &rootPath);
        bool createDisabled();
        void setCreateDisabled(bool disabled);
        bool checkInDisabled();
        void setCheckInDisabled(bool disabled);
        bool checkOutDisabled();
        void setCheckOutDisabled(bool disabled);

    private:
        //variables
        //Wt
        Wt::WVBoxLayout *_layMain;
        Wt::WToolBar *_tbMain;
        Wt::WContainerWidget *_cntTbMain;
        Wt::WHBoxLayout *_layCntTbMain;
        Wt::WPushButton *_btnCreate;
        Wt::WPushButton *_btnCheckIn;
        Wt::WPushButton *_btnCheckOut;
        Wt::WPushButton *_btnRefresh;
        Wt::WPushButton *_btnView;
        Wt::WContainerWidget *_cntFiles;
        Wt::WHBoxLayout *_layFiles;
        Wt::WTree *_trDirs;
        Wt::WTableView *_tblFiles;
        Wt::WStandardItemModel *_mdlTblFiles;
        Wt::WPushButton *_btnClose;

        //variables
        Log::Logger *_logger;
        std::string _rootPath;
        std::string _absoluteRootPath;
        bool _createEnabled;
        bool _checkInEnabled;
        bool _checkOutEnabled;

        std::vector<Ms::IO::MStreamedFileResource*> _downloadedFiles;

        //slots
        void _btnCreateClicked();
        void _btnCheckInClicked();
        void _btnCheckOutClicked();
        void _btnRefreshClicked();
        void _btnViewClicked();
        void _btnCloseClicked();
        void _trDirsItemSelectionChanged();

        //functions
        void _createDirTree();
        void _populateDirTree();
        void _createFilesTable();
        std::string _formatSize(u_int64_t size);
        void _refresh();
        std::string _createNewVersionDir(const std::string &path);
        std::string _generateDownloadUrl();
        std::string _getUniqueTmpFileName();
        bool _isDownloadingFiles();
        void _closeDownloadedFiles();
        void _prepareView();
    };
}

#endif // DLGFILESMANAGER_H
