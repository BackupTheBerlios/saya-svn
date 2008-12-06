/***************************************************************
 * Name:      iocommon.h
 * Purpose:   Implements Basic Cross-platform I/O Functions
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-06-14
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   WxWindows License
 * Comments:  This is a small reimplementation of some
 *            commonly used wxWidgets functions. The functions
 *            were written from scratch based on existing
 *            documentation and web searches, so they need
 *            testing before being released.
 **************************************************************/

#include "iocommon.h"
#include <cstdio>
#include <cstdarg>
#include <string.h>
#include <string>

using namespace std;

// The following string functions were taken from a personal string library of mine - Rick.
string rtrim(const string& str,const string& chars) {
  if( chars.empty() || str.empty() ) return str;
  int i;

  for(i=str.length();i>0;i--) {
    if(chars.find(str.substr(i-1,1),0)==string::npos) break;
  }
  return str.substr(0,i);
}

string ltrim(const string& str,const string& chars = " \r\n\t") {
  if( chars.empty() || str.empty() ) return str;
  unsigned int i;

  for(i=0;i<str.length();i++) {
    if(chars.find(str.substr(i,1),0)==string::npos) break;
  }
  return str.substr(i,str.length());
}

string trim(const string& str,const string& chars = " \r\n\t")
{
  return ltrim(rtrim(str,chars),chars);
}

syString ioCommon::GetPathname(const char* fullpath) {
  string s(fullpath);
  unsigned int i1 = s.rfind("/",s.length());
  unsigned int i2 = s.rfind("\\",s.length());
  unsigned int i;
  syString result;

  i = ( (i1 == string::npos) || (i2 != string::npos && (i2 > i1) ) ) ? i2 : i1;

  if(i==string::npos) {
    result = "";
  } else {
    result = trim(rtrim(s.substr(0,i),"/\\")).c_str();
  }
  return result;
}

syString ioCommon::GetFilename(const char* fullpath) {
  string s(fullpath);
  unsigned int i1 = s.rfind("/",s.length());
  unsigned int i2 = s.rfind("\\",s.length());
  unsigned int i;
  syString result;

  i = ( (i1==string::npos) || (i2!=string::npos && (i2 > i1) ) ) ? i2 : i1;
  if(i!=string::npos) {
    result = fullpath;
  } else {
    result = trim(s.substr(i+1,s.length())).c_str();
  }
  return result;
}
// End of string functions

const char* ioCommon::GetSeparator() {
    #ifdef __WIN32__
    static const char* result = "\\";
    #else
    static const char* result = "/";
    #endif
    return result;
}

bool ioCommon::FileExists(const char* filename) {
   FILE * infile;
   bool result = false;
   infile = fopen (filename, "r");

   if (infile != NULL)   {
      result = true;
   }
   fclose(infile);
   return result;
}

bool ioCommon::FileExists(const syString& filename) {
    return FileExists(filename.c_str());
}

bool ioCommon::DeleteFile(const char* filename) {
    if(!ioCommon::FileExists(filename)) return false;
    return ( ::remove(filename) != 0 );
}

bool ioCommon::DeleteFile(const syString& filename) {
    return DeleteFile(filename.c_str());
}

bool ioCommon::RenameFile(const char* oldname, const char* newname) {
    return ( ::rename(oldname, newname) != 0 );
}

const syString ioCommon::GetTemporaryFilename(const char* path, const char* prefix) {
    string filename;
    string fntemplate;
    string sprefix(prefix);
    unsigned int i;
    unsigned int i_max = 1000000;
    if(sprefix.empty()) {
        sprefix = "_tmp";
    }
    fntemplate = rtrim(path,"\\/");
    if(!fntemplate.empty()) {
        fntemplate.append(ioCommon::GetSeparator());
    }
    fntemplate.append(prefix);
    for(i = 0; i < i_max; i++) {
        filename = fntemplate + string(ioCommon::Printf("%6d",i).c_str());
        if(!ioCommon::FileExists(filename.c_str())) break; // Success!
    }
    if(i >= i_max) {
        filename.clear(); // Failed
    }
    return syString(filename.c_str());
}

const syString ioCommon::Printf(const char* format, ... ) {
    syString s;
    va_list arguments;
    unsigned int numchars;
    unsigned long bufsize = 2048; // We have to set a limit. 2K should be enough for most strings
    char* buffer;
    buffer = new char[bufsize + 1];

    // vsnprintf is a version of sprintf that takes a variable number of arguments. Additionally,
    // it allows you to set a limit on the buffer size used for storing the resulting string.
    // See http://linux.about.com/library/cmd/blcmdl3_vsnprintf.htm

    va_start(arguments, format);
    numchars = vsnprintf(buffer, bufsize, format, arguments);
    va_end(arguments);

    buffer[bufsize] = 0;

    if(numchars < bufsize) {
        buffer[numchars] = 0;
    }
    s = buffer;
    delete[] buffer;
    return s;
}

const syString ioCommon::PrintfBig(unsigned long bufsize, const char* format, ... ) {
    syString s;
    va_list arguments;
    unsigned int numchars;
    if(bufsize <= 1) {
        return syString("");
    } else {
        char* buffer = new char[bufsize]; // For big strings
        va_start(arguments, format);
        numchars = vsnprintf(buffer, bufsize - 1, format, arguments);
        va_end(arguments);
        if(numchars < bufsize - 1) {
            buffer[numchars] = 0;
        } else {
            buffer[bufsize - 1] = 0;
        }
        s = syString(buffer);
        delete[] buffer;
    }
    return s;
}

// *** FFile ***

FFile::FFile() : m_file(NULL) {
}

FFile::FFile(const char* filename, const char* mode) {
    m_file = fopen(filename, mode);
}

FFile::~FFile() {
    Close();
}

void FFile::Attach(void* fp) {
    m_file = fp;
}

bool FFile::Close() {
    int result = 0;
    if(m_file != NULL) {
        result = fclose((FILE*)m_file);
        m_file = NULL;
    }
    return (result == 0);
}

void FFile::Detach() {
    m_file = NULL;
}

void* FFile::fp() {
    return m_file;
}

bool FFile::Eof() {
    if(m_file == NULL)
        return true;
    return feof((FILE*)m_file);
}

bool FFile::Error() {
    if(m_file == NULL) {
        return false;
    }
    bool result = (ferror((FILE*)m_file) != 0);
    clearerr((FILE*)m_file);
    return result;
}

bool FFile::Flush() {
    if(m_file == NULL) {
        return false;
    }
    int result = fflush((FILE*)m_file);
    return (result == 0);
}

bool FFile::IsOpened() {
    return (m_file != NULL);
}

long FFile::Length() {
    if (m_file == NULL)
        return 0;
    long curlen;
    long oldpos = Tell();
    SeekEnd();
    curlen = Tell();
    Seek(oldpos);
    return curlen;
}

bool FFile::Open(const char* filename, const char* mode) {
    m_file = (void*)fopen(filename, mode);
    return IsOpened();
}

unsigned int FFile::Read(void* buffer, unsigned int count) {
    if(m_file == NULL) {
        return 0;
    }
    return fread(buffer, 1, count, (FILE*)m_file);
}

bool FFile::ReadAll(syString& str) {
    if(m_file == NULL) {
        return false;
    }
    long len = Length();
    bool result = false;
    str = "";
    char* buffer;
    if(len > 0) {
        buffer = new char[len + 1];
        if(Read(buffer, len)) {
            result = true;
        }
        buffer[len] = 0;
        str = buffer;
        delete[] buffer;
    }
    return result;
}

bool FFile::Seek(long ofs, ioCommon::SeekType mode) {
    if(m_file == NULL) {
        return false;
    }
    int origin;
    switch(mode) {
        case ioCommon::FromCurrent:
            origin = SEEK_CUR;
        break;
        case ioCommon::FromEnd:
            origin = SEEK_END;
        break;
        case ioCommon::FromStart:
        default:
            origin = SEEK_SET;
    }
    int result = fseek((FILE*)m_file, ofs, origin);
    return (result == 0);
}

bool FFile::SeekEnd(long ofs)  {
    return Seek(ofs, ioCommon::FromEnd);
}

long FFile::Tell() {
    if(m_file == NULL)
        return 0;
    return ftell((FILE*)m_file);
}

unsigned int FFile::Write(const void* buffer, unsigned int count) {
    if(m_file == NULL)
        return 0;
    return fwrite(buffer,1,count,(FILE*)m_file);
}

bool FFile::Write(const char* s) {
    return Write((const void*)(s), strlen(s));
}

// *** TempFile ***

TempFile::TempFile() :
m_filename(""),
m_tempfilename("")
{ }

TempFile::TempFile(const char* filename) {
    Open(filename);
}

bool TempFile::Open(const char* filename) {
    bool result = false;
    Discard();
    m_filename = filename;
    syString pathname = ioCommon::GetPathname(filename);
    m_tempfilename = ioCommon::GetTemporaryFilename(pathname.c_str());
    if(!m_tempfilename.empty()) {
        result = m_File.Open(m_tempfilename.c_str(),"wb+");
    }
    return result;
}

bool TempFile::IsOpened() {
    return m_File.IsOpened();
}

long TempFile::Length() {
    return m_File.Length();
}

long TempFile::Seek(long ofs, ioCommon::SeekType mode) {
    return m_File.Seek(ofs,mode);
}

long TempFile::Tell() {
    return m_File.Tell();
}

bool TempFile::Write(const void *p, unsigned int n) {
    return m_File.Write(p, n);
}

bool TempFile::Write(const char* str) {
    return m_File.Write(str);
}

bool TempFile::Commit(){
    bool result = false;
    bool exists = false;
    syString discardedfilename;
    syString pathname;

    do {
        if(!IsOpened()) break;
        m_File.Close();
        if( !ioCommon::FileExists(m_tempfilename.c_str()) ) break;

        // Now to rename the files.

        // Step 1: Rename destination file to a new temporary filename
        pathname = ioCommon::GetPathname(m_tempfilename.c_str());
        discardedfilename = ioCommon::GetTemporaryFilename(pathname.c_str());
        if( ioCommon::FileExists(m_filename)) {
            exists = true;
            if( !ioCommon::RenameFile(m_filename.c_str(), discardedfilename.c_str()) ) break;
        }

        // Step 2: Rename source file to destination file
        if( !ioCommon::RenameFile(m_tempfilename.c_str(), m_filename.c_str()) ) {

            // On error, reverse Step 1.
            ioCommon::RenameFile(discardedfilename.c_str(), m_filename.c_str());
            break;
        }

        // Step 3: Get rid of old renamed file.
        if(exists) {
            ioCommon::DeleteFile(discardedfilename.c_str());
            // Error checking this step is too expensive. The only reason would be that the file is readonly,
            // but that's most probably the user's fault. Let's just let the garbage accumulate in the current path.
        }
        result = true;
    }while(false);
    if(!result) {
        Discard(); // Get rid of the new file, since it's already closed.
    }
    return result;
}

void TempFile::Discard(){
    if(IsOpened()) {
        m_File.Close();
        ioCommon::DeleteFile(m_tempfilename);
    }
    m_filename.clear();
    m_tempfilename.clear();
}

TempFile::~TempFile(){
    Discard();
}
