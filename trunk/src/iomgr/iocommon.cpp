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
  size_t i;

  for(i=0;i<str.length();i++) {
    if(chars.find(str.substr(i,1),0)==string::npos) break;
  }
  return str.substr(i,str.length());
}

string trim(const string& str,const string& chars = " \r\n\t")
{
  return ltrim(rtrim(str,chars),chars);
}

std::string ioCommon::GetPathname(const std::string& fullpath) {
  unsigned int i1 = fullpath.rfind("/",fullpath.length());
  unsigned int i2 = fullpath.rfind("\\",fullpath.length());
  unsigned int i;
  string result;

  i = ( (i1 == string::npos) || (i2 != string::npos && (i2 > i1) ) ) ? i2 : i1;

  if(i==string::npos) {
    result = "";
  } else {
    result = trim(rtrim(fullpath.substr(0,i),"/\\"));
  }
  return result;
}

std::string ioCommon::GetFilename(const std::string& fullpath) {
  unsigned int i1 = fullpath.rfind("/",fullpath.length());
  unsigned int i2 = fullpath.rfind("\\",fullpath.length());
  unsigned int i;
  string result;

  i = ( (i1==string::npos) || (i2!=string::npos && (i2 > i1) ) ) ? i2 : i1;
  if(i!=string::npos) {
    result = fullpath;
  } else {
    result = trim(fullpath.substr(i+1,fullpath.length()));
  }

  return result;
}
// End of string functions

std::string ioCommon::GetSeparator() {
    #ifdef __WIN32__
        return "\\";
    #else
        return "/";
    #endif
}

bool ioCommon::FileExists(const std::string& filename) {
   return ioCommon::FileExists(filename.c_str());
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

bool ioCommon::DeleteFile(const std::string& filename) {
    return ioCommon::DeleteFile(filename.c_str());
}

bool ioCommon::DeleteFile(const char* filename) {
    if(!ioCommon::FileExists(filename)) return false;
    return ( ::remove(filename) != 0 );
}

bool ioCommon::RenameFile(const std::string& oldname, const std::string& newname) {
    return ioCommon::RenameFile(oldname.c_str(), newname.c_str());
}

bool ioCommon::RenameFile(const char* oldname, const char* newname) {
    return ( ::rename(oldname, newname) != 0 );
}

const std::string ioCommon::GetTemporaryFilename(const char* path, const char* prefix) {
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
        filename = fntemplate + ioCommon::Printf("%6d",i);
        if(!ioCommon::FileExists(filename)) break; // Success!
    }
    if(i >= i_max) {
        filename.clear(); // Failed
    }
    return filename;
}

const std::string ioCommon::GetTemporaryFilename(const std::string& path, const std::string& prefix) {
    return ioCommon::GetTemporaryFilename(path.c_str(), prefix.c_str());
}

const string ioCommon::Printf(const char* format, ... ) {
    string s;
    va_list arguments;
    unsigned int numchars;
    unsigned long bufsize = 2048; // We have to set a limit. 2K should be enough for most strings
    char* buffer;
    buffer = new char[bufsize + 1];
    numchars = vsnprintf(buffer, bufsize, format, arguments);
    buffer[bufsize] = 0;
    // vsnprintf is a version of sprintf that takes a variable number of arguments. Additionally,
    // it allows you to set a limit on the buffer size used for storing the resulting string.
    // See http://linux.about.com/library/cmd/blcmdl3_vsnprintf.htm

    if(numchars < bufsize) {
        buffer[numchars] = 0;
    }
    s = string(buffer);
    delete[] buffer;
    return s;
}

const string ioCommon::PrintfBig(unsigned long bufsize, const char* format, ... ) {
    string s;
    va_list arguments;
    unsigned int numchars;
    if(bufsize <= 1) {
        return "";
    } else {
        char buffer[bufsize]; // For big strings
        numchars = vsnprintf(buffer, bufsize - 1, format, arguments);
        if(numchars < bufsize - 1) {
            buffer[numchars] = 0;
        } else {
            buffer[bufsize - 1] = 0;
        }
        s = string(buffer);
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

void FFile::Attach(FILE* fp) {
    m_file = fp;
}

bool FFile::Close() {
    int result = 0;
    if(m_file != NULL) {
        result = fclose(m_file);
        m_file = NULL;
    }
    return (result == 0);
}

void FFile::Detach() {
    m_file = NULL;
}

FILE* FFile::fp() {
    return m_file;
}

bool FFile::Eof() {
    if(m_file == NULL)
        return true;
    return feof(m_file);
}

bool FFile::Error() {
    if(m_file == NULL) {
        return false;
    }
    bool result = (ferror(m_file) != 0);
    clearerr(m_file);
    return result;
}

bool FFile::Flush() {
    if(m_file == NULL) {
        return false;
    }
    int result = fflush(m_file);
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
    m_file = fopen(filename, mode);
    return IsOpened();
}

bool FFile::Open(const string& filename, const char* mode) {
    m_file = fopen(filename.c_str(), mode);
    return IsOpened();
}

size_t FFile::Read(void* buffer, size_t count) {
    if(m_file == NULL) {
        return 0;
    }
    return fread(buffer, 1, count, m_file);
}

bool FFile::ReadAll(string* str) {
    if(m_file == NULL) {
        return false;
    }
    long len = Length();
    bool result = false;
    str->clear();
    char* buffer;
    if(len > 0) {
        buffer = new char[len + 1];
        if(Read(buffer, len)) {
            result = true;
        }
        buffer[len] = 0;
        str->assign((const char*)buffer);
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
    int result = fseek(m_file, ofs, origin);
    return (result == 0);
}

bool FFile::SeekEnd(long ofs)  {
    return Seek(ofs, ioCommon::FromEnd);
}

long FFile::Tell() {
    if(m_file == NULL)
        return 0;
    return ftell(m_file);
}

size_t FFile::Write(const void* buffer, size_t count) {
    if(m_file == NULL)
        return 0;
    return fwrite(buffer,1,count,m_file);
}

bool FFile::Write(const string& s) {
    return Write((const void*)(s.c_str()), s.length());
}

// *** TempFile ***

TempFile::TempFile() :
m_filename(""),
m_tempfilename("")
{ }

TempFile::TempFile(const char* filename) {
    Open(filename);
}

TempFile::TempFile(const string& filename) {
    Open(filename);
}

bool TempFile::Open(const char* filename) {
    bool result = false;
    Discard();
    m_filename.assign(filename);
    std::string pathname = ioCommon::GetPathname(filename);
    m_tempfilename = ioCommon::GetTemporaryFilename(pathname);
    if(!m_tempfilename.empty()) {
        result = m_File.Open(m_tempfilename,"wb+");
    }
    return result;
}

bool TempFile::Open(const string& filename) {
    return Open(filename.c_str());
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

bool TempFile::Write(const void *p, size_t n) {
    return m_File.Write(p, n);
}

bool TempFile::Write(const string& str) {
    return m_File.Write(str);
}

bool TempFile::Commit(){
    bool result = false;
    bool exists = false;
    std::string discardedfilename;
    std::string pathname;

    do {
        if(!IsOpened()) break;
        m_File.Close();
        if( !ioCommon::FileExists(m_tempfilename) ) break;

        // Now to rename the files.

        // Step 1: Rename destination file to a new temporary filename
        pathname = ioCommon::GetPathname(m_tempfilename);
        discardedfilename = ioCommon::GetTemporaryFilename(pathname);
        if( ioCommon::FileExists(m_filename)) {
            exists = true;
            if( !ioCommon::RenameFile(m_filename, discardedfilename) ) break;
        }

        // Step 2: Rename source file to destination file
        if( !ioCommon::RenameFile(m_tempfilename, m_filename) ) {

            // On error, reverse Step 1.
            ioCommon::RenameFile(discardedfilename, m_filename);
            break;
        }

        // Step 3: Get rid of old renamed file.
        if(exists) {
            ioCommon::DeleteFile(discardedfilename);
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
