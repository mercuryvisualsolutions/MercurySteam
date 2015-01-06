#ifndef MFILEUPLOADDIALOG_H
#define MFILEUPLOADDIALOG_H

#include "iostream"
#include "map"

#include "Wt/WDialog"
#include "Wt/WFileUpload"
#include "Wt/WProgressBar"
#include "Wt/WPushButton"
#include "Wt/WLabel"
#include "Wt/WVBoxLayout"

namespace Ms
{
    namespace Widgets
    {
        namespace Dialogs
        {
            class MFilesUploadDialog : public Wt::WDialog
            {
            public:
                MFilesUploadDialog();
                MFilesUploadDialog(bool selectMultipleFiles, bool keepFiles);

                //functions
                const std::map<std::string,std::string> uploadedFilesMap() const;//contains names of the uploaded files in the form <serverFileName,ClientFileName>
                const std::vector<Wt::Http::UploadedFile> httpUploadedFiles() const;
                const std::string spoolFileName() const;
                const std::string clientFileName() const;
                bool isMultibleSelectionEnabled();
                void setMultibleSelectionEnabled(bool enabled);
                bool keepFiles();
                void setKeepFiles(bool keepFiles);

            private:
                //variables
                bool _keepFiles;

                //Wt
                Wt::WVBoxLayout *m_layContents;
                Wt::WFileUpload *m_filesUpload;
                Wt::WProgressBar *m_prgsBarUpload;
                Wt::WLabel *m_lblStatus;
                Wt::WPushButton *m_btnUpload;
                Wt::WPushButton *m_btnCancel;

                void prepareView();

                //slots
                void btnUploadClicked();
                void uploadSuccessfull();
                void fileTooLarge();
            };
        }
    }
}
#endif // MFILEUPLOADDIALOG_H
