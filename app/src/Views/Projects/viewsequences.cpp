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
    m_logger = Session::SessionManager::instance().logger();

    prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *Views::ViewSequences::qtvSequences() const
{
    return m_qtvSequences;
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

        m_qtvSequences->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvSequences->clearColumns();

        //add columns
        m_qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        m_qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Sequence_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        m_qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        m_qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Duration_In_Frames", "Duration In Frames", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        m_qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("FPS", "FPS", flags, new Ms::Widgets::Delegates::MFloatFieldDelegate(editRank)));
        m_qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Width", "Frame Width", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        m_qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Height", "Frame Height", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        m_qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        m_qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        m_qtvSequences->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));


        if(AppSettings::instance().isShowExtraColumns())
            m_qtvSequences->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvSequences->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating sequences view" << std::endl;
    }
}

bool Views::ViewSequences::isCreateOptionHidden()
{
    if(m_btnCreateSequence)
        return m_btnCreateSequence->isHidden();

    return false;
}

void Views::ViewSequences::setCreateOptionHidden(bool hidden) const
{
    if(m_btnCreateSequence)
        m_btnCreateSequence->setHidden(hidden);
}

bool Views::ViewSequences::isEditOptionHidden()
{
    if(m_btnEditSequences)
        return m_btnEditSequences->isHidden();

    return false;
}

void Views::ViewSequences::setEditOptionHidden(bool hidden) const
{
    if(m_btnEditSequences)
        m_btnEditSequences->setHidden(hidden);
}

//bool Views::ViewSequences::isRemoveOptionHidden()
//{
//    return m_btnRemoveSequences->isHidden();
//}

//void Views::ViewSequences::setRemoveOptionHidden(bool hidden) const
//{
//    m_btnRemoveSequences->setHidden(hidden);
//}

bool Views::ViewSequences::isImportThumbnailsOptionHidden()
{
    if(m_btnImportThumbnails)
        return m_btnImportThumbnails->isHidden();

    return false;
}

void Views::ViewSequences::setImportThumbnailsOptionHidden(bool hidden) const
{
    if(m_btnImportThumbnails)
        m_btnImportThumbnails->setHidden(hidden);
}

bool Views::ViewSequences::isOpenFilesOptionHidden()
{
    if(m_btnOpenFilesView)
        return m_btnOpenFilesView->isHidden();

    return false;
}

void Views::ViewSequences::setOpenFilesOptionHidden(bool hidden) const
{
    if(m_btnOpenFilesView)
        m_btnOpenFilesView->setHidden(hidden);
}

void Views::ViewSequences::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateProjectsPriv = user->hasPrivilege("Create Projects");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    m_btnCreateSequence->setHidden(!hasCreateProjectsPriv);
    m_btnImportThumbnails->setHidden(!hasEditPriv);
    m_btnEditSequences->setHidden(!hasEditPriv);

    m_qtvSequences->setImportCSVFeatureEnabled(hasCreateProjectsPriv);

    bool showTaskFilesButton = hasViewFilesPriv || hasCheckInPriv || hasCheckOutPriv || hasCreateRepoPriv;//if have any of the privileges
    m_btnOpenFilesView->setHidden(!showTaskFilesButton);
}

Wt::Signal<> &Views::ViewSequences::createSequenceRequested()
{
    return m_createSequenceRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &Views::ViewSequences::removeSequencesRequested()
{
    return m_removeSequencesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &Views::ViewSequences::editSequencesRequested()
{
    return m_editSequencesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &Views::ViewSequences::openfilesViewRequested()
{
    return m_openfilesViewRequested;
}

Wt::Signal<> &Views::ViewSequences::importThumbnailsRequested()
{
    return m_importThumbnailsRequested;
}

void Views::ViewSequences::btnCreateSequenceClicked()
{
    m_createSequenceRequested();
}

void Views::ViewSequences::btnRemoveSequencesClicked()
{
    m_removeSequencesRequested(m_qtvSequences->selectedItems());
}

void Views::ViewSequences::btnEditSequencesClicked()
{
    if(m_qtvSequences->table()->selectedIndexes().size() == 0)
    {
        m_logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditSequence *dlg = new Views::DlgCreateAndEditSequence(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(auto seqPtr : m_qtvSequences->selectedItems())
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

            m_qtvSequences->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewSequences::btnImportThumbnailsClicked()
{
    if(m_qtvSequences->model()->rowCount() == 0)
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
                    for(int i = 0; i < m_qtvSequences->model()->rowCount(); ++i)
                    {
                        Wt::Dbo::ptr<Projects::ProjectSequence> seqPtr = m_qtvSequences->model()->resultRow(i);

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
                    m_logger->log(std::string("Error occured while trying to import thumbnails to table sequences") + e.what(),
                                 Ms::Log::LogMessageType::Error);
                }

                delFiles.push_back(pair.first);//cache it for later deletion
            }

            transaction.commit();

            for(std::vector<std::string>::size_type i = 0; i < delFiles.size(); ++i)
            {
                Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
                m_logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
            }

            m_qtvSequences->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

    m_importThumbnailsRequested();
}

void Views::ViewSequences::btnOpenFilesViewClicked()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(m_qtvSequences->table()->selectedIndexes().size() != 1)
    {
        m_logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Wt::Dbo::ptr<Projects::ProjectSequence> seqPtr = m_qtvSequences->selectedItems().at(0);

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

    m_openfilesViewRequested(m_qtvSequences->selectedItems());
}

void Views::ViewSequences::sequenceDataAboutToBeChanged(const Wt::WModelIndex &index, const boost::any &value, int role)
{
    //get sequence
    Wt::Dbo::ptr<Projects::ProjectSequence> sequencePtr = m_qtvSequences->itemForModelIndex(index);

    std::string headerName = Wt::asString(index.model()->headerData(index.column())).toUTF8();
    std::string orgValue = Wt::asString(index.data(role)).toUTF8();
    std::string newValue = Wt::asString(value).toUTF8();

    if(orgValue == newValue)
        return;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string message = "Sequence \""  + sequencePtr->name() + "\" in project \"" +
            sequencePtr->projectName() + "\" \"" + headerName + "\" has changed from \"" + orgValue + "\" to \"" + newValue + "\"";

    Database::Notification *notification = new Database::Notification(message);

    try
    {
        Wt::Dbo::ptr<Database::Notification> notificationPtr = Session::SessionManager::instance().dboSession().createDbo<Database::Notification>(notification);

        if(notificationPtr.get())
        {
            //notify all users of tasks in current sequence of the change
            for(auto iter = sequencePtr->tasks().begin(); iter != sequencePtr->tasks().end(); ++iter)
            {
                (*iter)->user().modify()->addNotification(notificationPtr);
            }
        }
        else
        {
            delete notification;

            m_logger->log("Error occured while trying to create new notification", Ms::Log::LogMessageType::Error);
        }
    }
    catch(Wt::Dbo::Exception ex)
    {
        delete notification;

        m_logger->log(ex.what(), Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        delete notification;

        m_logger->log("Error occured while trying to create new notification", Ms::Log::LogMessageType::Error);
    }
}

void Views::ViewSequences::createSequencesTableView()
{
    m_qtvSequences = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectSequence>(Session::SessionManager::instance().dboSession());
    m_qtvSequences->setRowHeight(160);
    m_qtvSequences->setDefaultFilterColumnIndex(1);
    m_qtvSequences->setIgnoreNumFilterColumns(1);

    Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<Projects::ProjectSequence>> *model = const_cast<Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<Projects::ProjectSequence>>*>(m_qtvSequences->model());

    model->dataAboutToBeChanged().connect(this, &Views::ViewSequences::sequenceDataAboutToBeChanged);

    m_btnCreateSequence = m_qtvSequences->createToolButton("", "icons/Add.png", "Create A New Sequence");
    m_btnCreateSequence->clicked().connect(this, &Views::ViewSequences::btnCreateSequenceClicked);

    //m_btnRemoveSequences = m_qtvSequences->createToolButton("", "icons/Remove.png", "Remove Selected Sequences");
    //m_btnRemoveSequences->clicked().connect(this, &Views::ViewSequences::btnRemoveSequencesClicked);

    m_btnEditSequences = m_qtvSequences->createToolButton("", "icons/Edit.png", "Edit Selected Sequences");
    m_btnEditSequences->clicked().connect(this, &Views::ViewSequences::btnEditSequencesClicked);

    m_btnImportThumbnails= m_qtvSequences->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
    m_btnImportThumbnails->clicked().connect(this, &Views::ViewSequences::btnImportThumbnailsClicked);

    m_btnOpenFilesView = m_qtvSequences->createToolButton("", "icons/Files.png", "Files Manager");
    m_btnOpenFilesView->clicked().connect(this, &Views::ViewSequences::btnOpenFilesViewClicked);
}

void Views::ViewSequences::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    createSequencesTableView();

    m_layMain->addWidget(m_qtvSequences);
}
