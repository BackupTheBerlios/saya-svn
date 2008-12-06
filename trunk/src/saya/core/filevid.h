/***************************************************************
 * Name:      filevid.h
 * Purpose:   Declaration for the FileVID class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-11-17
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef filevid_h
#define filevid_h

#include "videoinputdevice.h"
class syString;

// TODO: Add support to add "file video handlers"
// TODO: Complete implementation of FileVID

class FileVIDData;

/**
 * FileVID is a derivate of VideoInputDevice. It handles video files.
 */
class FileVID : public VideoInputDevice {
    friend class FileVIDData;
    public:

        /** Standard constructor. */
        FileVID();

        /** Standard destructor. */
        virtual ~FileVID();

        /** Sets the file to load on Init(). (const char* version) */
        bool SetFile(const char* filename);

        /** Sets the file to load on Init(). (syString version) */
        bool SetFile(const syString& filename);

        /** Gets the file being processed */
        syString GetFile();

        /** @brief Allocates memory for m_Bitmap. Called by Init().
         *  @note If you override this function, remember to call it in your derived class' AllocateResources()
         */
        virtual bool AllocateResources();

        /** @brief Releases the memory for m_Bitmap. Called by ShutDown().
         *  @note If you override this function, remember to call it in your derived class' FreeResources().
         */
        virtual void FreeResources();

    protected:
        void LoadCurrentFrame();

    private:
        FileVIDData* m_Data;
};

#endif
