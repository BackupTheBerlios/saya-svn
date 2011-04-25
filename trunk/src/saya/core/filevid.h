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

#include "avsource.h"
class syString;

// TODO: Complete implementation of FileVID

/**
 * FileVID is a derivate of VideoInputDevice. It handles video files.
 */
class FileVID : public AVSource {
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

        /** @brief Loads the current frame into m_Bitmap.
         *
         *  This is a stub; you need to override this function to acomplish anything.
         *  @warning You MUST NOT call Seek() from LoadCurrentFrame(), or you will trigger a mutex deadlock!!
         *  If you need to do a seeking, call InternalSeek() instead.
         */
        virtual void LoadCurrentFrame();

        /** @brief Seeks to a determinate instant in time.
         *
         *  @param time The time, in avtime_t units, to seek to.
         *  @return The current instant in time where the current frame will be read.
         *  @note  This function is called by InternalSeek().
         *  @warning This function MUST NOT update m_CurrentTime. That is done by InternalSeek().
         */
        virtual avtime_t SeekVideoResource(avtime_t time);

        virtual const syBitmap* GetBitmap();

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
