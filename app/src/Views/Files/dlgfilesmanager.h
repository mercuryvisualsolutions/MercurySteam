#ifndef DLGFILESMANAGER_H
#define DLGFILESMANAGER_H

#include "../../Session/sessionmanager.h"
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

#include <Ms/Widgets/MTableViewWidget.h>
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
        bool viewDisabled();
        void setViewDisabled(bool disabled);

    private:
        //variables
        //Wt
        Wt::WVBoxLayout *m_layMain;
        Wt::WToolBar *m_tbMain;
        Wt::WContainerWidget *m_cntTbMain;
        Wt::WHBoxLayout *m_layCntTbMain;
        Wt::WPushButton *m_btnCreate;
        Wt::WPushButton *m_btnCheckIn;
        Wt::WPushButton *m_btnCheckOut;
        Wt::WPushButton *m_btnRefresh;
        Wt::WPushButton *m_btnView;
        Wt::WContainerWidget *m_cntFiles;
        Wt::WHBoxLayout *m_layFiles;
        Wt::WTree *m_trDirs;
        Ms::Widgets::MTableViewWidget *m_tblFiles;
        Wt::WPushButton *m_btnClose;

        //variables
        
        Log::Logger *m_logger;
        std::string m_rootPath;
        std::string m_absoluteRootPath;
        bool m_createEnabled;
        bool m_checkInEnabled;
        bool m_checkOutEnabled;
        bool m_isViewDisabled;

        std::vector<Ms::IO::MStreamedFileResource*> m_downloadedFiles;

        //slots
        void btnCreateClicked();
        void btnCheckInClicked();
        void btnCheckOutClicked();
        void btnRefreshClicked();
        void btnViewClicked();
        void btnCloseClicked();
        void trDirsItemSelectionChanged();
        void tblFilesItemDoubleClicked(Wt::WModelIndex index);

        //functions
        void viewItem(Wt::WModelIndex index);
        void createDirTree();
        void populateDirTree();
        void createFilesTable();
        std::string formatSize(u_int64_t size);
        void refresh();
        std::string createNewVersionDir(const std::string &path);
        std::string generateDownloadUrl();
        std::string getUniqueTmpFileName();
        bool isDownloadingFiles();
        void closeDownloadedFiles();
        void prepareView();
    };
}

#endif // DLGFILESMANAGER_H
