#ifndef VIDPROJECT_H
#define VIDPROJECT_H


class VidProject
{
    public:
        VidProject();
        virtual ~VidProject();
        bool LoadFromXml(const wxString &data);
    protected:
    private:
};

#endif // VIDPROJECT_H
