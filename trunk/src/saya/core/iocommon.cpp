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
#include "systring.h"
#include <cstdio>
#include <string.h>

syString ioCommon::GetPathname(const char* fullpath) {
  return GetPathname(syString(fullpath,true));
}

syString ioCommon::GetPathname(const syString& fullpath) {
  int i1 = fullpath.rfind("/");
  int i2 = fullpath.rfind("\\");
  int i;
  syString result;

  i = ( (i1 == syString::npos) || (i2 != syString::npos && (i2 > i1) ) ) ? i2 : i1;

  if(i==syString::npos) {
    result = "";
  } else {
    result = trim(rtrim(fullpath.substr(0,i),"/\\"));
  }
  return result;
}

syString ioCommon::GetFilename(const char* fullpath) {
  return GetFilename(syString(fullpath, true));
}

syString ioCommon::GetFilename(const syString& fullpath) {
  int i1 = fullpath.rfind("/");
  int i2 = fullpath.rfind("\\");
  int i;
  syString result;

  i = ( (i1==syString::npos) || (i2!=syString::npos && (i2 > i1) ) ) ? i2 : i1;
  if(i==syString::npos) {
    result = fullpath;
  } else {
    result = trim(fullpath.substr(i+1,fullpath.length()));
  }
  return result;
}

// End of String functions

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
      fclose(infile);
   }
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
    syString filename;
    syString fntemplate;
    syString sprefix(prefix);
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
        filename = fntemplate + syString::Format("%6d",i);
        if(!ioCommon::FileExists(filename)) break; // Success!
    }
    if(i >= i_max) {
        filename.clear(); // Failed
    }
    return filename;
}


// *** FFile ***
class FFile::Data {
    public:
        Data();
        void* m_File;
        syString m_Filename;
};

FFile::Data::Data() :
m_File(NULL),
m_Filename("")
{
}

FFile::FFile() {
    m_Data = new Data;
}

FFile::FFile(const char* filename, const char* mode) {
    m_Data = new Data;
    m_Data->m_File = fopen(filename, mode);
}

FFile::~FFile() {
    Close();
    delete m_Data;
}

void FFile::Attach(void* fp) {
    m_Data->m_File = fp;
}

bool FFile::Close() {
    int result = 0;
    if(m_Data->m_File != NULL) {
        result = fclose((FILE*)m_Data->m_File);
        m_Data->m_File = NULL;
    }
    return (result == 0);
}

void FFile::Detach() {
    m_Data->m_File = NULL;
}

void* FFile::fp() {
    return m_Data->m_File;
}

bool FFile::Eof() {
    if(m_Data->m_File == NULL)
        return true;
    return feof((FILE*)m_Data->m_File);
}

bool FFile::Error() {
    if(m_Data->m_File == NULL) {
        return false;
    }
    bool result = (ferror((FILE*)m_Data->m_File) != 0);
    clearerr((FILE*)m_Data->m_File);
    return result;
}

bool FFile::Flush() {
    if(m_Data->m_File == NULL) {
        return false;
    }
    int result = fflush((FILE*)m_Data->m_File);
    return (result == 0);
}

bool FFile::IsOpened() {
    return (m_Data->m_File != NULL);
}

long FFile::Length() {
    if (m_Data->m_File == NULL)
        return 0;
    long curlen;
    long oldpos = Tell();
    SeekEnd();
    curlen = Tell();
    Seek(oldpos);
    return curlen;
}

bool FFile::Open(const char* filename, const char* mode) {
    m_Data->m_File = (void*)fopen(filename, mode);
    return IsOpened();
}

unsigned int FFile::Read(void* buffer, unsigned int count) {
    if(m_Data->m_File == NULL) {
        return 0;
    }
    return fread(buffer, 1, count, (FILE*)m_Data->m_File);
}

bool FFile::ReadAll(syString& str) {
    if(m_Data->m_File == NULL) {
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
    if(m_Data->m_File == NULL) {
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
    int result = fseek((FILE*)m_Data->m_File, ofs, origin);
    return (result == 0);
}

bool FFile::SeekEnd(long ofs)  {
    return Seek(ofs, ioCommon::FromEnd);
}

long FFile::Tell() {
    if(m_Data->m_File == NULL)
        return 0;
    return ftell((FILE*)m_Data->m_File);
}

unsigned int FFile::Write(const void* buffer, unsigned int count) {
    if(m_Data->m_File == NULL)
        return 0;
    return fwrite(buffer,1,count,(FILE*)m_Data->m_File);
}

bool FFile::Write(const char* s) {
    return Write((const void*)(s), strlen(s));
}

bool FFile::Write(const syString& s) {
    return Write(s.c_str(),s.length());
}

// *** TempFile ***

class TempFile::Data {
    public:
        Data();
        FFile m_File;
        syString m_Filename;
        syString m_TempFilename;
};

TempFile::Data::Data() :
m_Filename(""),
m_TempFilename("")
{
}

TempFile::TempFile() {
    m_Data = new Data;
}

TempFile::~TempFile(){
    Discard();
    delete m_Data;
}


TempFile::TempFile(const char* filename) {
    m_Data = new Data;
    Open(filename);
}

bool TempFile::Open(const char* filename) {
    bool result = false;
    Discard();
    m_Data->m_Filename = filename;
    syString pathname = ioCommon::GetPathname(filename);
    m_Data->m_TempFilename = ioCommon::GetTemporaryFilename(pathname.c_str());
    if(!m_Data->m_TempFilename.empty()) {
        result = m_Data->m_File.Open(m_Data->m_TempFilename.c_str(),"wb+");
    }
    return result;
}

bool TempFile::IsOpened() {
    return m_Data->m_File.IsOpened();
}

long TempFile::Length() {
    return m_Data->m_File.Length();
}

long TempFile::Seek(long ofs, ioCommon::SeekType mode) {
    return m_Data->m_File.Seek(ofs,mode);
}

long TempFile::Tell() {
    return m_Data->m_File.Tell();
}

bool TempFile::Write(const void *p, unsigned int n) {
    return m_Data->m_File.Write(p, n);
}

bool TempFile::Write(const char* str) {
    return m_Data->m_File.Write(str);
}

bool TempFile::Write(const syString& s) {
    return Write(s.c_str(),s.length());
}

bool TempFile::Commit(){
    bool result = false;
    bool exists = false;
    syString discardedfilename;
    syString pathname;

    do {
        if(!IsOpened()) break;
        m_Data->m_File.Close();
        if( !ioCommon::FileExists(m_Data->m_TempFilename.c_str()) ) break;

        // Now to rename the files.

        // Step 1: Rename destination file to a new temporary filename
        pathname = ioCommon::GetPathname(m_Data->m_TempFilename.c_str());
        discardedfilename = ioCommon::GetTemporaryFilename(pathname.c_str());
        if( ioCommon::FileExists(m_Data->m_Filename)) {
            exists = true;
            if( !ioCommon::RenameFile(m_Data->m_Filename.c_str(), discardedfilename.c_str()) ) break;
        }

        // Step 2: Rename source file to destination file
        if( !ioCommon::RenameFile(m_Data->m_TempFilename.c_str(), m_Data->m_Filename.c_str()) ) {

            // On error, reverse Step 1.
            ioCommon::RenameFile(discardedfilename.c_str(), m_Data->m_Filename.c_str());
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
        m_Data->m_File.Close();
        ioCommon::DeleteFile(m_Data->m_TempFilename);
    }
    m_Data->m_Filename.clear();
    m_Data->m_TempFilename.clear();
}

