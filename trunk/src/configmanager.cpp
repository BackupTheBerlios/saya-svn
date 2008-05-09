#include "configmanager.h"


// Configmanager is based upon the following variables:

// ID - the variable id
// realid - the variable's realid. The variable's 4 bits

#include <vector>

using namespace std;

class config_id_node;

struct config_id_data {
    unsigned int real_id;
    char type_id;
    config_id_node* nodeinfo;
};

class config_id_node {
    public:
        unsigned int id;
        vector <unsigned int> children;
};

class config_pool {
    public:
        config_pool();
        ~config_pool() {}

        bool GetUInt(unsigned int id,unsigned int &value);
        bool GetChar(unsigned int id,char &value);
        bool GetwxChar(unsigned int id,wxChar &value);
        bool GetByte(unsigned int id,unsigned char &value);
        bool GetBool(unsigned int id,bool &value);
        bool GetFloat(unsigned int id,float &value);
        bool GetDouble(unsigned int id,double &value);
        bool GetString(unsigned int id,wxString &value);


};

static configmanager *the_configmanager = 0;

/** @brief configmanager
  *
  * Constructor
  */
 configmanager::configmanager()
{

}

/** @brief SetString
  *
  * @todo: document this function
  */
bool configmanager::SetString(unsigned int id,const wxString &value)
{

}

/** @brief SetDouble
  *
  * @todo: document this function
  */
bool configmanager::SetDouble(unsigned int id,double value)
{

}

/** @brief SetFloat
  *
  * @todo: document this function
  */
bool configmanager::SetFloat(unsigned int id,float value)
{

}

/** @brief SetBool
  *
  * @todo: document this function
  */
bool configmanager::SetBool(unsigned int id,bool value)
{

}

/** @brief SetByte
  *
  * @todo: document this function
  */
bool configmanager::SetByte(unsigned int id,unsigned char value)
{

}

/** @brief SetwxChar
  *
  * @todo: document this function
  */
bool configmanager::SetwxChar(unsigned int id,wxChar value)
{

}

/** @brief SetWchar
  *
  * @todo: document this function
  */
bool configmanager::SetWchar(unsigned int id,wchar_t value)
{

}

/** @brief SetChar
  *
  * @todo: document this function
  */
bool configmanager::SetChar(unsigned int id,char value)
{

}

/** @brief SetUInt
  *
  * @todo: document this function
  */
bool configmanager::SetUInt(unsigned int id,unsigned int value)
{

}

/** @brief SetInt
  *
  * @todo: document this function
  */
bool configmanager::SetInt(unsigned int id,int value)
{

}

/** @brief GetString
  *
  * @todo: document this function
  */
bool configmanager::GetString(unsigned int id,wxString &value)
{

}

/** @brief GetDouble
  *
  * @todo: document this function
  */
bool configmanager::GetDouble(unsigned int id,double &value)
{

}

/** @brief GetFloat
  *
  * @todo: document this function
  */
bool configmanager::GetFloat(unsigned int id,float &value)
{

}

/** @brief GetBool
  *
  * @todo: document this function
  */
bool configmanager::GetBool(unsigned int id,bool &value)
{

}

/** @brief GetByte
  *
  * @todo: document this function
  */
bool configmanager::GetByte(unsigned int id,unsigned char &value)
{

}

/** @brief GetWchar
  *
  * @todo: document this function
  */
bool configmanager::GetWchar(unsigned int id,wchar_t &value)
{

}

/** @brief GetwxChar
  *
  * @todo: document this function
  */
bool configmanager::GetwxChar(unsigned int id,wxChar &value)
{

}

/** @brief GetChar
  *
  * @todo: document this function
  */
bool configmanager::GetChar(unsigned int id,char &value)
{

}

/** @brief GetUInt
  *
  * @todo: document this function
  */
bool configmanager::GetUInt(unsigned int id,unsigned int &value)
{

}

/** @brief GetInt
  *
  * @todo: document this function
  */
bool configmanager::GetInt(unsigned int id,int &value)
{

}

/** @brief GetType
  *
  * @todo: document this function
  */
char configmanager::GetType(unsigned int id)
{

}

/** @brief SaveConfigToXmlString
  *
  * @todo: document this function
  */
bool configmanager::SaveConfigToXmlString(const wxString section,wxString &data)
{

}

/** @brief SaveConfigToXml
  *
  * @todo: document this function
  */
bool configmanager::SaveConfigToXml(const wxString section,const wxString filename)
{

}

/** @brief SaveConfig
  *
  * @todo: document this function
  */
bool configmanager::SaveConfig(const wxString section, const wxString filename)
{

}

/** @brief LoadConfigFromXmlString
  *
  * @todo: document this function
  */
bool configmanager::LoadConfigFromXmlString(const wxString section,const wxString data)
{

}

/** @brief LoadConfigFromXml
  *
  * @todo: document this function
  */
bool configmanager::LoadConfigFromXml(const wxString section,const wxString filename)
{

}

/** @brief LoadConfig
  *
  * @todo: document this function
  */
bool configmanager::LoadConfig(const wxString section, const wxString filename)
{

}

/** @brief GenStringId
  *
  * @todo: document this function
  */
unsigned int configmanager::GenStringId(const wxString path,const wxString defaultvalue)
{

}

/** @brief GenDoubleId
  *
  * @todo: document this function
  */
unsigned int configmanager::GenDoubleId(const wxString path,double defaultvalue)
{

}

/** @brief GenFloatId
  *
  * @todo: document this function
  */
unsigned int configmanager::GenFloatId(const wxString path,float defaultvalue)
{

}

/** @brief GenBoolId
  *
  * @todo: document this function
  */
unsigned int configmanager::GenBoolId(const wxString path,bool defaultvalue)
{

}

/** @brief GenByteId
  *
  * @todo: document this function
  */
unsigned int configmanager::GenByteId(const wxString path,unsigned char defaultvalue)
{

}

/** @brief GenwxCharId
  *
  * @todo: document this function
  */
unsigned int configmanager::GenwxCharId(const wxString path,wxChar defaultvalue)
{

}

/** @brief GenWcharId
  *
  * @todo: document this function
  */
unsigned int configmanager::GenWcharId(const wxString path,wchar_t defaultvalue)
{

}

/** @brief GenCharId
  *
  * @todo: document this function
  */
unsigned int configmanager::GenCharId(const wxString path,char defaultvalue)
{

}

/** @brief GenUIntId
  *
  * @todo: document this function
  */
unsigned int configmanager::GenUIntId(const wxString path,unsigned int defaultvalue)
{

}

/** @brief GenIntId
  *
  * @todo: document this function
  */
unsigned int configmanager::GenIntId(const wxString path,int defaultvalue)
{

}

/** @brief unload
  *
  * unloads the configmanager
  */
void configmanager::unload()
{
    if(the_configmanager) {
        delete the_configmanager;
        the_configmanager = 0;
    }

}

/** @brief get
  *
  * Creates a single instance of configmanager
  */
configmanager configmanager::get()
{
    if(!the_configmanager) {
        the_configmanager = new configmanager();
    }
}

/** @brief ~configmanager
  *
  * destructor
  */
 configmanager::~configmanager()
{

}

/** @brief config_pool
  *
  * @todo: document this function
  */
 config_pool::config_pool()
{

}

/** @brief GetUInt
  *
  * @todo: document this function
  */
bool config_pool::GetUInt(unsigned int id,unsigned int &value)
{

}

/** @brief GetChar
  *
  * @todo: document this function
  */
bool config_pool::GetChar(unsigned int id,char &value)
{

}

/** @brief GetwxChar
  *
  * @todo: document this function
  */
bool config_pool::GetwxChar(unsigned int id,wxChar &value)
{

}

/** @brief GetByte
  *
  * @todo: document this function
  */
bool config_pool::GetByte(unsigned int id,unsigned char &value)
{

}

/** @brief GetBool
  *
  * @todo: document this function
  */
bool config_pool::GetBool(unsigned int id,bool &value)
{

}

/** @brief GetFloat
  *
  * @todo: document this function
  */
bool config_pool::GetFloat(unsigned int id,float &value)
{

}

/** @brief GetDouble
  *
  * @todo: document this function
  */
bool config_pool::GetDouble(unsigned int id,double &value)
{

}

/** @brief GetString
  *
  * @todo: document this function
  */
bool config_pool::GetString(unsigned int id,wxString &value)
{

}

