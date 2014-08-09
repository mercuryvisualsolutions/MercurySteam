#ifndef VIEWASSETS_H
#define VIEWASSETS_H

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

        //Signals
        Wt::Signal<> &createAssetRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> &removeAssetsRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> &openfilesViewRequested();
        Wt::Signal<> &importThumbnailsRequested();

    private:
        //Variables
        //UI
        Wt::WPushButton *_btnCreateAsset;
        Wt::WPushButton *_btnRemoveAssets;
        Wt::WPushButton *_btnEditAssets;
        Wt::WPushButton *_btnImportThumbnails;
        Wt::WPushButton *_btnOpenFilesView;

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *_qtvAssets;
        Log::Logger *_logger;
        //UI variables
        Wt::WVBoxLayout *_layMain;

        //Signals
        Wt::Signal<> _createAssetRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> _removeAssetsRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> _openfilesViewRequested;
        Wt::Signal<> _importThumbnailsRequested;

        //Slots
        void _btnCreateAssetClicked();
        void _btnRemoveAssetsClicked();
        void _btnEditAssetsClicked();
        void _btnImportThumbnailsClicked();
        void _btnOpenFilesViewClicked();

        //Functions
        void _createAssetsTableView();
        void _prepareView();
    };
}

#endif // VIEWASSETS_H
