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

// TODO: Complete implementation of FileVID

/**
 * FileVID is a derivate of VideoInputDevice. It handles video files.
 */
class FileVID : public VideoInputDevice {
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

        /** @brief Gets the frame corresponding to the given time.
         *  @param time Instant in time where we want to get the frame index.
         *  @return The frame index (zero-based) corresponding to the given time.
         *  @note This function overrides VideoInputDevice::GetFrameIndex.
         */
        virtual unsigned long GetFrameIndex(avtime_t time);

        /** @brief Gets the time corresponding to a given frame.
         *  @param The frame index (zero-based) where we want to get the time.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The instant in time from the video start corresponding to the given frame.
         *  @note This function overrides VideoInputDevice::GetTimeFromFrameIndex.
         */
        virtual avtime_t GetTimeFromFrameIndex(unsigned long  frame, bool fromend = false);

    protected:
        void LoadCurrentFrame();

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
