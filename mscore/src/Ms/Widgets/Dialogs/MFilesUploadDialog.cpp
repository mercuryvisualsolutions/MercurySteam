#include "MFilesUploadDialog.h"

Ms::Widgets::Dialogs::MFilesUploadDialog::MFilesUploadDialog()
{
    prepareView();
}

Ms::Widgets::Dialogs::MFilesUploadDialog::MFilesUploadDialog(bool selectMultipleFiles, bool keepFiles) :
    MFilesUploadDialog()
{
    m_filesUpload->setMultiple(selectMultipleFiles);
    _keepFiles = keepFiles;
}

const std::vector<Wt::Http::UploadedFile> Ms::Widgets::Dialogs::MFilesUploadDialog::httpUploadedFiles() const
{
    return m_filesUpload->uploadedFiles();
}

const std::string Ms::Widgets::Dialogs::MFilesUploadDialog::spoolFileName() const
{
    return m_filesUpload->spoolFileName();
}

const std::string Ms::Widgets::Dialogs::MFilesUploadDialog::clientFileName() const
{
    return m_filesUpload->clientFileName().toUTF8();
}

bool Ms::Widgets::Dialogs::MFilesUploadDialog::isMultibleSelectionEnabled()
{
    return m_filesUpload->multiple();
}

void Ms::Widgets::Dialogs::MFilesUploadDialog::setMultibleSelectionEnabled(bool enabled)
{
    m_filesUpload->setMultiple(enabled);
}

bool Ms::Widgets::Dialogs::MFilesUploadDialog::keepFiles()
{
    return _keepFiles;
}

void Ms::Widgets::Dialogs::MFilesUploadDialog::setKeepFiles(bool keepFiles)
{
    _keepFiles = keepFiles;
}

const std::map<std::string, std::string> Ms::Widgets::Dialogs::MFilesUploadDialog::uploadedFilesMap() const
{
    std::map<std::string,std::string> filesMap;
    for(const Wt::Http::UploadedFile &file : m_filesUpload->uploadedFiles())
    {
        filesMap[file.spoolFileName()] = file.clientFileName();
        std::cout << "uploaded file:" << file.spoolFileName() << std::endl;
    }

    return filesMap;
}

void Ms::Widgets::Dialogs::MFilesUploadDialog::prepareView()
{
    this->setCaption("Upload Files");
    this->rejectWhenEscapePressed(true);

    m_layContents = new Wt::WVBoxLayout();
    m_layContents->setContentsMargins(0,0,0,0);
    m_layContents->setSpacing(0);

    this->contents()->setLayout(m_layContents);

    m_prgsBarUpload = new Wt::WProgressBar();

    m_filesUpload = new Wt::WFileUpload();
    m_filesUpload->stealSpooledFile();
    m_filesUpload->setFileTextSize(40);
    m_filesUpload->setProgressBar(m_prgsBarUpload);
    m_filesUpload->uploaded().connect(this, &Dialogs::MFilesUploadDialog::uploadSuccessfull);
    m_filesUpload->fileTooLarge().connect(this, &Dialogs::MFilesUploadDialog::fileTooLarge);

    m_layContents->addWidget(m_filesUpload);

    m_lblStatus = new Wt::WLabel();
    m_layContents->addWidget(m_lblStatus);

    m_btnUpload = new Wt::WPushButton("Upload", this->footer());
    m_btnUpload->clicked().connect(this, &Dialogs::MFilesUploadDialog::btnUploadClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

void Ms::Widgets::Dialogs::MFilesUploadDialog::btnUploadClicked()
{
    m_btnUpload->setDisabled(false);
    m_btnCancel->setDisabled(false);

    m_filesUpload->upload();

    m_lblStatus->setText("Uploading...");
}

void Ms::Widgets::Dialogs::MFilesUploadDialog::uploadSuccessfull()
{
    m_lblStatus->setText("Upload successfull");
    if(_keepFiles)
    {
        for(const Wt::Http::UploadedFile &file : m_filesUpload->uploadedFiles())
        {
            file.stealSpoolFile();
        }
    }

    if(m_filesUpload->uploadedFiles().size() > 0)
        this->accept();
    else
        this->reject();
}

void Ms::Widgets::Dialogs::MFilesUploadDialog::fileTooLarge()
{
    std::cerr << "One of the uploaded files is too large" << std::endl;
    m_lblStatus->setText("One of the uploaded files is too large");

    m_btnUpload->setDisabled(false);
    m_btnCancel->setDisabled(false);
}
