#ifndef VIEWASSETS_H
#define VIEWASSETS_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"
#include "../../Log/logger.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewAssets : public Wt::WContainerWidget
    {
    public:
        ViewAssets();

        //functions
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *qtvAssets() const;
        void updateView(const std::vector<Wt::Dbo::ptr<Projects::Project>> &prjVec) const;

        bool isCreateOptionHidden();
        void setCreateOptionHidden(bool hidden) const;
        bool isRemoveOptionHidden();
        void setRemoveOptionHidden(bool hidden) const;
        bool isEditOptionHidden();
        void setEditOptionHidden(bool hidden) const;
        bool isImportThumbnailsOptionHidden();
        void setImportThumbnailsOptionHidden(bool hidden) const;
        bool isOpenFilesOptionHidden();
        void setOpenFilesOptionHidden(bool hidden) const;

        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<> &createAssetRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> &removeAssetsRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> &openfilesViewRequested();
        Wt::Signal<> &importThumbnailsRequested();

    private:
        //Variables
        
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *m_qtvAssets;
        Log::Logger *m_logger;
        //UI
        Wt::WPushButton *m_btnCreateAsset;
        Wt::WPushButton *m_btnRemoveAssets;
        Wt::WPushButton *m_btnEditAssets;
        Wt::WPushButton *m_btnImportThumbnails;
        Wt::WPushButton *m_btnOpenFilesView;
        Wt::WVBoxLayout *m_layMain;

        //Signals
        Wt::Signal<> m_createAssetRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> m_removeAssetsRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> m_openfilesViewRequested;
        Wt::Signal<> m_importThumbnailsRequested;

        //Slots
        void btnCreateAssetClicked();
        void btnRemoveAssetsClicked();
        void btnEditAssetsClicked();
        void btnImportThumbnailsClicked();
        void btnOpenFilesViewClicked();

        void assetDataAboutToBeChanged(const Wt::WModelIndex &index, const boost::any &value, int role);

        //Functions
        void createAssetsTableView();
        //requires active transaction
        void sendTaskNotification(Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr, const std::string &property, const std::string &orgValue, const std::string &newValue);
        void prepareView();
    };
}

#endif // VIEWASSETS_H
