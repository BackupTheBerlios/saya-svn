/***************************************************************
 * Name:      iocommon.h
 * Purpose:   Defines Basic Cross-platform I/O Functions
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

#ifndef iocommon_h
#define iocommon_h

class syString;

/** @brief Class for File handling functions
  *
  */
class ioCommon {
    public:

        /** Used for indicating where to seek from */
        enum SeekType {
            FromStart = 0,
            FromCurrent = 1,
            FromEnd
        };

        /** Standard constructor */
        ioCommon() {}

        /** Standard destructor */
        virtual ~ioCommon() {}

        /** Gets the path separator ( / or \\ )for the current OS. */
        static const char* GetSeparator();

        /** @brief Gets the pathname for a filename.
          *
          * @param fullpath The full path to extract the pathname from.
          * @return the pathname of the file.
          */
        static syString GetPathname(const char* fullpath);

        /** @brief Gets the basename (filename and extension) for a filename.
          *
          * @param fullpath The full path to extract the basename from.
          * @return the name of the file, without path component.
          */
        static syString GetFilename(const char* fullpath);

        /** @brief Checks for the existance of a file.
          *
          * @param filename the filename to be checked
          * @return true if the file exists; false otherwise.
          */
        static bool FileExists(const char* filename);

        /** @brief Checks for the existance of a file.
          *
          * @param filename the filename to be checked
          * @return true if the file exists; false otherwise.
          */
        static bool FileExists(const syString& filename);

        /** @brief Deletes a file.
          *
          * @param filename the filename to be deleted
          * @return true if the file was deleted successfully; false otherwise.
          */
        static bool DeleteFile(const char* filename);

        /** @brief Deletes a file.
          *
          * @param filename the filename to be deleted
          * @return true if the file was deleted successfully; false otherwise.
          */
        static bool DeleteFile(const syString& filename);

        /** @brief Renames a file.
          *
          * @param oldname The file to be renamed
          * @param newname The new name
          * @return true on success; false otherwise.
          */
        static bool RenameFile(const char* oldname, const char* newname);

        /** @brief Creates a temporary filename with the given prefix
          *
          * @param path The path where the temporary file should be created
          * @param prefix a prefix to prepend to the filename.
          * @return the resulting filename.
          * @warning Using this function could lead to a race condition between checking the file's existence and
          * opening the file.
          */
        static const syString GetTemporaryFilename(const char* path, const char* prefix = "_sayatmp");
};


/** @brief Generic buffered file object.
  *
  * Originally designed as a wxFFile wrapper, this object will allow us to access files
  * and write them more efficiently using a memory buffer.
  * @see wxFFile
  */
class FFile {
    public:

        /** Standard constructor */
        FFile();

        /** Constructor with a filename as parameter
          *
          * @param filename The file to read and/or write
          * @param mode The I/O mode for opening the file.
          */
        FFile(const char* filename, const char* mode = "r");

        /** Standard destructor. */
        ~FFile();

        /** Attachs an existing FILE handle to the object. We use (void*) for compilation speedup purposes. */
        void Attach(void* fp);

        /** Closes the file. */
        bool Close();

        /** Detachs the FILE handle from the object. */
        void Detach();

        /** Gets the FILE handle. We use (void*) for compilation speedup purposes. */
        void* fp();

        /** Have we reached the End of File? */
        bool Eof();

        /** True if there was an error in the I/O. */
        bool Error();

        /** Flushes the buffer into the file. */
        bool Flush();

        /** Returns true if the file was opened successfully. */
        bool IsOpened();

        /** Returns the file length in bytes. */
        long Length();

        /** @brief Opens a file.
          *
          * @param filename The file to open
          * @param mode The I/O mode that will be used to open the file.
          * @return true on success; false otherwise.
          */
        bool Open(const char* filename, const char* mode = "r");

        /** @brief Reads data from the file.
          *
          * @param buffer A pointer indicating the destination buffer.
          * @param count The number of bytes to read.
          * @return true on success; false otherwise.
          */
        unsigned int Read(void* buffer, unsigned int count);

        /** @brief Reads all data from the file into a string object.
          *
          * @param str The destination string.
          * @return true on success; false otherwise.
          * @warning For big files this could end up using all memory. Use with care.
          */
        bool ReadAll(syString& dest);

        /** @brief Seeks to a specific offset in the file.
          *
          * @param ofs The offset to go to.
          * @param mode Where to seek from. @see ioCommon::SeekType
          */
        bool Seek(long ofs, ioCommon::SeekType mode = ioCommon::FromStart);

        /** @brief Seeks to a specific offset in the file from the EOF.
          * @param ofs The offset to go to.
          */
        bool SeekEnd(long ofs = 0);

        /** Returns the current position in the file. */
        long Tell();

        /** @brief Writes data into the file.
          *
          * @param buffer Points to a buffer containing the data.
          * @param count  The number of bytes to write.
          * @return The number of bytes written.
          */
        unsigned int Write(const void* buffer, unsigned int count);

        /** @brief Writes data into the file (const-char* version).
          *
          * @param s The data to be written.
          * @return true on success; false otherwise.
          */
        bool Write(const char* s);

        /** @brief Writes data into the file.
          *
          * @param str An syString object to write into the file.
          */
        bool Write(const syString& s);


    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

/** @brief Generic temporary file object.
  *
  * Originally designed as a wxTempFile wrapper, this object will allow us to create temporary files
  * to replace existing ones when the data is finished being written.
  * @see wxTempFile
  */
class TempFile {
    public:

        /** Standard constructor */
        TempFile();

        /** Constructor with a const char* as parameter
          * @param filename The file to save the data into when the commit takes place.
          */
        TempFile(const char* filename);

        /** @brief Opens a file for writing. The file will be replaced when the commit takes place.
          *
          * @param filename The file to save the data into when the commit takes place.
          */
        bool Open(const char* filename);

        /** Is the file opened correctly? */
        bool IsOpened();

        /** Current length of the file */
        long Length();

        /** @brief Moves the file pointer to a specific offset.
          *
          * @param ofs The offset to move to.
          * @param mode Where to seek. 0 = From the beginning; 1 = From the current pointer, 2 = from the end of file.
          */
        long Seek(long ofs, ioCommon::SeekType mode = ioCommon::FromStart);

        /** Gets the current file pointer. */
        long Tell();

        /** @brief Writes data into the file.
          *
          * @param p A pointer indicating where the data is.
          * @param
          */
        bool Write(const void *p, unsigned int n);

        /** @brief Writes data into the file.
          *
          * @param str A c-string to write into the file.
          */
        bool Write(const char* str);

        /** @brief Writes data into the file.
          *
          * @param str An syString object to write into the file.
          */
        bool Write(const syString& s);

        /** @brief Saves the data under the specified file.
          *
          * @return true on success; false otherwise.
          */
        bool Commit();

        /** Discards the written data. The original file is untouched. */
        void Discard();

        /** Standard destructor. */
        ~TempFile();
    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
