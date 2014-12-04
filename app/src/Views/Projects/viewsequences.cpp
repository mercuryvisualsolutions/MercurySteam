#include "viewsequences.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"
#include "../../Projects/projectsio.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Log/logmanager.h"
#include "dlgcreateandeditsequence.h"

#include <Wt/WApplication>

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewSequences::ViewSequences()
{
    _logger = Session::SessionManager::instance().logger();

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
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectSequence>> query = Session::SessionManager::instance().dboSession().find<Projects::ProjectSequence>();

        if(prjVec.size() > 0)
        {
            std::vector<std::string> projectsIdValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::Project>(prjVec);

            //generate the where clause
            query.where("Sequence_Project_Project_Name IN (" + projectsIdValues.at(0) + ")");

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Session::SessionManager::instance().user()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("Sequence_Name = ?").bind("");//clear the view

        _qtvSequences->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        _qtvSequences->clearColumns();

        //add columns
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Sequence_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Duration_In_Frames", "Duration In Frames", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("FPS", "FPS", flags, new Ms::Widgets::Delegates::MFloatFieldDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Width", "Frame Width", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Height", "Frame Height", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));


        if(AppSettings::instance().isShowExtraColumns())
            _qtvSequences->addBaseColumns(flags, editRank);

        transaction.commit();

        _qtvSequences->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating sequences view" << std::endl;
    }
}

bool Views::ViewSequences::isCreateOptionHidden()
{
    if(_btnCreateSequence)
        return _btnCreateSequence->isHidden();

    return false;
}

void Views::ViewSequences::setCreateOptionHidden(bool hidden) const
{
    if(_btnCreateSequence)
        _btnCreateSequence->setHidden(hidden);
}

bool Views::ViewSequences::isEditOptionHidden()
{
    if(_btnEditSequences)
        return _btnEditSequences->isHidden();

    return false;
}

void Views::ViewSequences::setEditOptionHidden(bool hidden) const
{
    if(_btnEditSequences)
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
    if(_btnImportThumbnails)
        return _btnImportThumbnails->isHidden();

    return false;
}

void Views::ViewSequences::setImportThumbnailsOptionHidden(bool hidden) const
{
    if(_btnImportThumbnails)
        _btnImportThumbnails->setHidden(hidden);
}

bool Views::ViewSequences::isOpenFilesOptionHidden()
{
    if(_btnOpenFilesView)
        return _btnOpenFilesView->isHidden();

    return false;
}

void Views::ViewSequences::setOpenFilesOptionHidden(bool hidden) const
{
    if(_btnOpenFilesView)
        _btnOpenFilesView->setHidden(hidden);
}

void Views::ViewSequences::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateProjectsPriv = user->hasPrivilege("Create Projects");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    _btnCreateSequence->setHidden(!hasCreateProjectsPriv);
    _btnImportThumbnails->setHidden(!hasEditPriv);
    _btnEditSequences->setHidden(!hasEditPriv);

    _qtvSequences->setImportCSVFeatureEnabled(hasCreateProjectsPriv);

    bool showTaskFilesButton = hasViewFilesPriv || hasCheckInPriv || hasCheckOutPriv || hasCreateRepoPriv;//if have any of the privileges
    _btnOpenFilesView->setHidden(!showTaskFilesButton);
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
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(auto seqPtr : _qtvSequences->selectedItems())
            {
                if(dlg->editedStartDate())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectSequence>(seqPtr)->setStartDate(dlg->startDate());

                if(dlg->editedEndDate())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectSequence>(seqPtr)->setEndDate(dlg->endDate());

                if(dlg->editedDuration())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectSequence>(seqPtr)->setDurationInFrames(dlg->duration());

                if(dlg->editedFps())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectSequence>(seqPtr)->setFps(dlg->fps());

                if(dlg->editedFrameWidth())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectSequence>(seqPtr)->setFrameWidth(dlg->frameWidth());

                if(dlg->editedFrameHeight())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectSequence>(seqPtr)->setFrameHeight(dlg->frameHeight());

                if(dlg->editedPriority())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectSequence>(seqPtr)->setPriority(dlg->priority());

                if(dlg->editedStatus())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectSequence>(seqPtr)->setStatus(dlg->status());

                if(dlg->editedDescription())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectSequence>(seqPtr)->setDescription(dlg->description());

                if(dlg->editedActive())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectSequence>(seqPtr)->setActive(dlg->isActive());
            }

            transaction.commit();

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
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            std::vector<std::string> delFiles;//holds thumbnails files for later deletion

            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(const std::pair<std::string,std::string> &pair : dlg->uploadedFilesMap())
            {
                try
                {
                    //get the original raw file name without the extension
                    int lastIndex = pair.second.find_last_of(".");
                    std::string rawFileName = pair.second.substr(0, lastIndex);

                    //match thumbnail by sequence name
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
                                Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectSequence>(seqPtr)->setThumbnail(Projects::ProjectsIO::getRelativeSequenceDir(seqPtr->projectName(), seqPtr->name()) +
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

            transaction.commit();

            for(std::vector<std::string>::size_type i = 0; i < delFiles.size(); ++i)
            {
                Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
                _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
            }

            _qtvSequences->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

    _importThumbnailsRequested();
}

void Views::ViewSequences::_btnOpenFilesViewClicked()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(_qtvSequences->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Wt::Dbo::ptr<Projects::ProjectSequence> seqPtr = _qtvSequences->selectedItems().at(0);

    DlgFilesManager *dlgFiles = new DlgFilesManager(Projects::ProjectsIO::getRelativeSequenceDir(seqPtr->projectName(), seqPtr->name()) + Ms::IO::dirSeparator() + "files");
    dlgFiles->finished().connect(std::bind([=]()
    {
        delete dlgFiles;
    }));

    Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    dlgFiles->setViewDisabled(!hasViewFilesPriv);
    dlgFiles->setCreateDisabled(!hasCreateRepoPriv);
    dlgFiles->setCheckInDisabled(!hasCheckInPriv);
    dlgFiles->setCheckOutDisabled(!hasCheckOutPriv);

    transaction.commit();

    dlgFiles->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

    _openfilesViewRequested(_qtvSequences->selectedItems());
}

void Views::ViewSequences::_createSequencesTableView()
{
    _qtvSequences = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectSequence>(Session::SessionManager::instance().dboSession());
    _qtvSequences->setRowHeight(160);
    _qtvSequences->setDefaultFilterColumnIndex(1);
    _qtvSequences->setIgnoreNumFilterColumns(1);

    _btnCreateSequence = _qtvSequences->createToolButton("", "icons/Add.png", "Create A New Sequence");
    _btnCreateSequence->clicked().connect(this, &Views::ViewSequences::_btnCreateSequenceClicked);

    //_btnRemoveSequences = _qtvSequences->createToolButton("", "icons/Remove.png", "Remove Selected Sequences");
    //_btnRemoveSequences->clicked().connect(this, &Views::ViewSequences::_btnRemoveSequencesClicked);

    _btnEditSequences = _qtvSequences->createToolButton("", "icons/Edit.png", "Edit Selected Sequences");
    _btnEditSequences->clicked().connect(this, &Views::ViewSequences::_btnEditSequencesClicked);

    _btnImportThumbnails= _qtvSequences->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
    _btnImportThumbnails->clicked().connect(this, &Views::ViewSequences::_btnImportThumbnailsClicked);

    _btnOpenFilesView = _qtvSequences->createToolButton("", "icons/Files.png", "Files Manager");
    _btnOpenFilesView->clicked().connect(this, &Views::ViewSequences::_btnOpenFilesViewClicked);
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
