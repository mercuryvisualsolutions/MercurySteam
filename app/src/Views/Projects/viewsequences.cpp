#include "viewsequences.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../../Projects/projectsio.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Log/logmanager.h"
#include "dlgcreateandeditsequence.h"

#include <Wt/WApplication>

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewSequences::ViewSequences()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());

    _prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *Views::ViewSequences::qtvSequences() const
{
    return _qtvSequences;
}

void Views::ViewSequences::updateView(const std::vector<Wt::Dbo::ptr<Projects::Project>> &prjVec) const
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectSequence>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectSequence>();

        if(prjVec.size() > 0)
        {
            std::vector<std::string> projectsIdValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::Project>(prjVec);

            //generate the where clause
            query.where("Sequence_Project_Project_Name IN (" + projectsIdValues.at(0) + ")");

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("Sequence_Name = ?").bind("");//clear the view

        _qtvSequences->setQuery(query);

        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        _qtvSequences->clearColumns();

        //add columns
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Sequence_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Duration_In_Frames", "Duration_In_Frames", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("FPS", "FPS", flags, new Ms::Widgets::Delegates::MFloatFieldDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Width", "Frame Width", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Height", "Frame Height", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));


        if(AppSettings::instance().isShowExtraColumns())
            _qtvSequences->addBaseColumns(flags, editRank);

        _qtvSequences->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating sequences view" << std::endl;
    }
}

bool Views::ViewSequences::isCreateOptionHidden()
{
    return _btnCreateSequence->isHidden();
}

void Views::ViewSequences::setCreateOptionHidden(bool hidden) const
{
    _btnCreateSequence->setHidden(hidden);
}

bool Views::ViewSequences::isEditOptionHidden()
{
    return _btnEditSequences->isHidden();
}

void Views::ViewSequences::setEditOptionHidden(bool hidden) const
{
    _btnEditSequences->setHidden(hidden);
}

//bool Views::ViewSequences::isRemoveOptionHidden()
//{
//    return _btnRemoveSequences->isHidden();
//}

//void Views::ViewSequences::setRemoveOptionHidden(bool hidden) const
//{
//    _btnRemoveSequences->setHidden(hidden);
//}

bool Views::ViewSequences::isImportThumbnailsOptionHidden()
{
    return _btnImportThumbnails->isHidden();
}

void Views::ViewSequences::setImportThumbnailsOptionHidden(bool hidden) const
{
    _btnImportThumbnails->setHidden(hidden);
}

bool Views::ViewSequences::isOpenFilesOptionHidden()
{
    return _btnOpenFilesView->isHidden();
}

void Views::ViewSequences::setOpenFilesOptionHidden(bool hidden) const
{
    _btnOpenFilesView->setHidden(hidden);
}

Wt::Signal<> &Views::ViewSequences::createSequenceRequested()
{
    return _createSequenceRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &Views::ViewSequences::removeSequencesRequested()
{
    return _removeSequencesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &Views::ViewSequences::editSequencesRequested()
{
    return _editSequencesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &Views::ViewSequences::openfilesViewRequested()
{
    return _openfilesViewRequested;
}

Wt::Signal<> &Views::ViewSequences::importThumbnailsRequested()
{
    return _importThumbnailsRequested;
}

void Views::ViewSequences::_btnCreateSequenceClicked()
{
    _createSequenceRequested();
}

void Views::ViewSequences::_btnRemoveSequencesClicked()
{
    _removeSequencesRequested(_qtvSequences->selectedItems());
}

void Views::ViewSequences::_btnEditSequencesClicked()
{
    if(_qtvSequences->table()->selectedIndexes().size() == 0)
    {
        _logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditSequence *dlg = new Views::DlgCreateAndEditSequence(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            for(auto seqPtr : _qtvSequences->selectedItems())
            {
                if(dlg->editedStartDate())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(seqPtr)->setStartDate(dlg->startDate());

                if(dlg->editedEndDate())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(seqPtr)->setEndDate(dlg->endDate());

                if(dlg->editedDuration())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(seqPtr)->setDurationInFrames(dlg->duration());

                if(dlg->editedFps())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(seqPtr)->setFps(dlg->fps());

                if(dlg->editedFrameWidth())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(seqPtr)->setFrameWidth(dlg->frameWidth());

                if(dlg->editedFrameHeight())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(seqPtr)->setFrameHeight(dlg->frameHeight());

                if(dlg->editedPriority())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(seqPtr)->setPriority(dlg->priority());

                if(dlg->editedStatus())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(seqPtr)->setStatus(dlg->status());

                if(dlg->editedDescription())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(seqPtr)->setDescription(dlg->description());

                if(dlg->editedActive())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(seqPtr)->setActive(dlg->isActive());
            }

            _qtvSequences->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewSequences::_btnImportThumbnailsClicked()
{
    if(_qtvSequences->model()->rowCount() == 0)
        return;

    Ms::Widgets::Dialogs::MFilesUploadDialog *dlg = new Ms::Widgets::Dialogs::MFilesUploadDialog(true, true);
    dlg->finished().connect(std::bind([=]()
    {
        std::vector<std::string> delFiles;//holds thumbnails files for later deletion
        for(const std::pair<std::string,std::string> &pair : dlg->uploadedFilesMap())
        {
            try
            {
                //get the original raw file name without the extension
                int lastIndex = pair.second.find_last_of(".");
                std::string rawFileName = pair.second.substr(0, lastIndex);

                //match thumbnail by sequence name
                if(!Database::DatabaseManager::instance().openTransaction())
                    return;

                //loop for all sequences
                for(int i = 0; i < _qtvSequences->model()->rowCount(); ++i)
                {
                    Wt::Dbo::ptr<Projects::ProjectSequence> seqPtr = _qtvSequences->model()->resultRow(i);

                    if(seqPtr->name() == rawFileName)//sequence has the same name of the thumbnail ?
                    {
                        if(seqPtr->active() == false && (!AppSettings::instance().isLoadInactiveData()))//don't change thumbnail for inactive sequence
                            continue;

                        std::string seqDir = Projects::ProjectsIO::getAbsoluteSequenceDir(seqPtr->projectName(), seqPtr->name());
                        std::string localFile = seqDir + Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second;

                        if(Ms::IO::fileExists(localFile))//if thumbnail already exist
                            Ms::IO::removeFile(localFile);//delete it

                        if(Ms::IO::copyFile(pair.first, localFile))//copy and rename the file to the original name
                        {
                            Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(seqPtr)->setThumbnail(Projects::ProjectsIO::getRelativeSequenceDir(seqPtr->projectName(), seqPtr->name()) +
                                    Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second);
                        }

                        break;//sequence mtching thumbnail name found, exit loop
                    }
                }
            }
            catch(Wt::WException e)
            {
                _logger->log(std::string("Error occured while trying to import thumbnails to table sequences") + e.what(),
                             Ms::Log::LogMessageType::Error);
            }

            delFiles.push_back(pair.first);//cache it for later deletion
        }

        for(std::vector<std::string>::size_type i = 0; i < delFiles.size(); ++i)
        {
            Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
            _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
        }

        _qtvSequences->updateView();

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

    _importThumbnailsRequested();
}

void Views::ViewSequences::_btnOpenFilesViewClicked()
{
    if(_qtvSequences->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Wt::Dbo::ptr<Projects::ProjectSequence> seqPtr = _qtvSequences->selectedItems().at(0);

    DlgFilesManager *dlg = new DlgFilesManager(Projects::ProjectsIO::getRelativeSequenceDir(seqPtr->projectName(), seqPtr->name()) + Ms::IO::dirSeparator() + "files");
    dlg->finished().connect(std::bind([=]()
    {
        delete dlg;
    }));

    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
        dlg->setCreateDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In"))
        dlg->setCheckInDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out"))
        dlg->setCheckOutDisabled(true);

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

    _openfilesViewRequested(_qtvSequences->selectedItems());
}

void Views::ViewSequences::_createSequencesTableView()
{
    _qtvSequences = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectSequence>(&Database::DatabaseManager::instance());
    _qtvSequences->setRowHeight(160);
    _qtvSequences->setDefaultFilterColumnIndex(1);
    _qtvSequences->setIgnoreNumFilterColumns(1);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        _btnCreateSequence = _qtvSequences->createToolButton("", "icons/Add.png", "Create A New Sequence");
        _btnCreateSequence->clicked().connect(this, &Views::ViewSequences::_btnCreateSequenceClicked);

        _qtvSequences->setImportCSVFeatureEnabled(true);
    }
    else
        _qtvSequences->setImportCSVFeatureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //_btnRemoveSequences = _qtvSequences->createToolButton("", "icons/Remove.png", "Remove Selected Sequences");
        //_btnRemoveSequences->clicked().connect(this, &Views::ViewSequences::_btnRemoveSequencesClicked);
    }

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        _btnEditSequences = _qtvSequences->createToolButton("", "icons/Edit.png", "Edit Selected Sequences");
        _btnEditSequences->clicked().connect(this, &Views::ViewSequences::_btnEditSequencesClicked);
    }

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        _btnImportThumbnails= _qtvSequences->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
        _btnImportThumbnails->clicked().connect(this, &Views::ViewSequences::_btnImportThumbnailsClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        _btnOpenFilesView = _qtvSequences->createToolButton("", "icons/Files.png", "Files Manager");
        _btnOpenFilesView->clicked().connect(this, &Views::ViewSequences::_btnOpenFilesViewClicked);
    }
}

void Views::ViewSequences::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _createSequencesTableView();

    _layMain->addWidget(_qtvSequences);
}
