#ifndef VIEWASSETS_H
#define VIEWASSETS_H

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewAssets : public Wt::WContainerWidget
    {
    public:
        ViewAssets();

        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *qtvAssets() const;

        //Signals
        Wt::Signal<> &createAssetRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> &removeAssetsRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> &openfilesViewRequested();
        Wt::Signal<> &importThumbnailsRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *_qtvAssets;
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
        void _btnImportThumbnailsClicked();
        void _btnOpenFilesViewClicked();

        //Functions
        void _createAssetsTableView();
        void _prepareView();
    };
}

#endif // VIEWASSETS_H
