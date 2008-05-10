#include "vidproject.h"

VidProject::VidProject()
{
    //ctor
}

VidProject::~VidProject()
{
    //dtor
}

bool VidProject::IsModified() {
    return m_IsModified;
}

void VidProject::SetModified() {
    m_IsModified = true;
}

bool VidProject::LoadFromXml(const wxString &data) {
// TODO (rick#1#): Implement VidProject::LoadFromXml
    m_IsModified = false;
    return true;
}

bool VidProject::SaveToXml(wxString &data) {
// TODO (rick#1#): Implement VidProject::SaveToXml
    return true;
}