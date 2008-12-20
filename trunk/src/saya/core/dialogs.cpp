/**************************************************************
 * Name:      core/dialogs.cpp
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Purpose:   Implements classes for GUI Dialogs boxes.
 **************************************************************/

#include <vector>
#include "systring.h"
#include "dialogs.h"
#include "app.h"

using namespace std;

// ------------------------------
// begin syFileDialogResult::Data
// ------------------------------

class syFileDialogResult::Data {
    public:
        Data();
        ~Data() {}
        Data(const syFileDialogResult::Data& copy);
        syFileDialogResult::Data& operator=(const syFileDialogResult::Data& copy);
        bool GetOKResult();
        unsigned int GetFileCount();
        void AddFile(const syString& filename);
        void Clear();
        const syString operator[](unsigned int idx);
    private:
        vector<syString> m_Files;
};

syFileDialogResult::Data::Data() {
    Clear();
}

syFileDialogResult::Data::Data(const syFileDialogResult::Data& copy) {
    m_Files = copy.m_Files;
}

syFileDialogResult::Data& syFileDialogResult::Data::operator=(const syFileDialogResult::Data& copy) {
    m_Files = copy.m_Files;
    return *this;
}

bool syFileDialogResult::Data::GetOKResult() {
    return m_Files.size() > 0;
}
unsigned int syFileDialogResult::Data::GetFileCount() {
    return m_Files.size();
}

void syFileDialogResult::Data::AddFile(const syString& filename) {
    for(unsigned int i = 0; i < m_Files.size(); ++i) {
        if(m_Files[i] == filename) {
            return;
        }
    }
    m_Files.push_back(filename);
}

void syFileDialogResult::Data::Clear() {
    m_Files.clear();
}
const syString syFileDialogResult::Data::operator[](unsigned int idx) {
    if(idx >= m_Files.size()) return "";
    return m_Files[idx];
}


// ----------------------------
// end syFileDialogResult::Data
// ----------------------------

syFileDialogResult::syFileDialogResult() :
m_Data(new Data)
{
}

syFileDialogResult::syFileDialogResult(const syFileDialogResult& copy) {
    m_Data = copy.m_Data;
}

syFileDialogResult& syFileDialogResult::operator=(const syFileDialogResult& copy) {
    m_Data = copy.m_Data;
    return *this;
}

syFileDialogResult::~syFileDialogResult() {
    delete m_Data;
}

bool syFileDialogResult::GetOKResult() {
    return m_Data->GetOKResult();
}

unsigned int syFileDialogResult::GetFileCount() {
    return m_Data->GetFileCount();
}

void syFileDialogResult::AddFile(const syString& filename) {
    return m_Data->AddFile(filename);
}

void syFileDialogResult::Clear() {
    m_Data->Clear();
}

const syString syFileDialogResult::operator[](unsigned int idx) {
    return m_Data->operator[](idx);
}

syFileDialogResult syFileSelector(
    const syString& message,
    const syString& default_path,
    const syString& default_filename,
    const syString& default_extension,
    const syString& wildcard,
    int flags,
    void* parent,
    int x,
    int y)
{
    return syApp::Get()->FileSelector(message,default_path,default_filename,default_extension,
        wildcard, flags, parent, x, y);
}

int syMessageBox(const syString& message, const syString& caption,unsigned int flags,void* parent) {
    return syApp::Get()->MessageBox(message, caption,flags, parent);
}

void syErrorMessageBox(const syString& message) {
    syApp::Get()->ErrorMessageBox(message);
}

void syLogStatus(const syString& message) {
    syApp::Get()->LogStatus(message);
}
