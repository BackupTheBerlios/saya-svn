#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class configmanager
{
    public:
        virtual ~configmanager();

        static configmanager get();
        static void unload();

        // The following functions Generate consecutive IDs at runtime
        static unsigned int GenIntId(const wxString path,int defaultvalue = 0);
        static unsigned int GenUIntId(const wxString path,unsigned int defaultvalue = 0);
        static unsigned int GenCharId(const wxString path,char defaultvalue = 0);
        static unsigned int GenWcharId(const wxString path,wchar_t defaultvalue = 0);
        static unsigned int GenwxCharId(const wxString path,wxChar defaultvalue = 0);
        static unsigned int GenByteId(const wxString path,unsigned char defaultvalue = 0);
        static unsigned int GenBoolId(const wxString path,bool defaultvalue = false);
        static unsigned int GenFloatId(const wxString path,float defaultvalue = 0);
        static unsigned int GenDoubleId(const wxString path,double defaultvalue = 0);
        static unsigned int GenStringId(const wxString path,const wxString defaultvalue = wxEmptyString);
        // NOTE: 0 = invalid id; 1 = root id (generated automatically)

        bool LoadConfig(const wxString section, const wxString filename); // if empty, it uses wxConfig storage method
        bool LoadConfigFromXml(const wxString section,const wxString filename);
        bool LoadConfigFromXmlString(const wxString section,const wxString data);

        bool SaveConfig(const wxString section, const wxString filename); // if empty, it uses wxConfig storage method
        bool SaveConfigToXml(const wxString section,const wxString filename);
        bool SaveConfigToXmlString(const wxString section,wxString &data);

        char GetType(unsigned int id); // Gets the data type for certain id

        bool GetInt(unsigned int id,int &value);
        bool GetUInt(unsigned int id,unsigned int &value);
        bool GetChar(unsigned int id,char &value);
        bool GetwxChar(unsigned int id,wxChar &value);
        bool GetWchar(unsigned int id,wchar_t &value);
        bool GetByte(unsigned int id,unsigned char &value);
        bool GetBool(unsigned int id,bool &value);
        bool GetFloat(unsigned int id,float &value);
        bool GetDouble(unsigned int id,double &value);
        bool GetString(unsigned int id,wxString &value);

        bool SetInt(unsigned int id,int value);
        bool SetUInt(unsigned int id,unsigned int value);
        bool SetChar(unsigned int id,char value);
        bool SetWchar(unsigned int id,wchar_t value);
        bool SetwxChar(unsigned int id,wxChar value);
        bool SetByte(unsigned int id,unsigned char value);
        bool SetBool(unsigned int id,bool value);
        bool SetFloat(unsigned int id,float value);
        bool SetDouble(unsigned int id,double value);
        bool SetString(unsigned int id,const wxString &value);

    protected:

    private:
        configmanager();
};

#endif // CONFIGMANAGER_H
