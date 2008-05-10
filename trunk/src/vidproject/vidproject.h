#ifndef VIDPROJECT_H
#define VIDPROJECT_H


class VidProject
{
    public:
        VidProject();
        virtual ~VidProject();
        bool LoadFromXml(const wxString &data);
        bool SaveToXml(wxString &data);
        bool IsModified();
        void SetModified();
    protected:
    private:
        bool m_IsModified;
};

#endif // VIDPROJECT_H
