/***************************************************************
 * Name:      videooutputdevice.h
 * Purpose:   Declaration    of the VideoOutputDevice class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef videooutputdevice_h
#define videooutputdevice_h

class syMutex;
/** enumerates the Video Color formats that our device can render / encode */
enum VideoColorFormat {
    vcfRGB8,
    vcfRGB32,
    vcfRGB24,
    vcfRGB16,
    vcfRGB15,
    vcfBGR15,
    vcfBGR16,
    vcfBGR24,
    vcfBGR32,
    vcfYUY2,
    vcfYUV,
    vcfYV12,
    vcfYUV12,
    vcfYVYU,
    vcfUYVY,
    vcfYUY9,
    vcfY800
};

/**
 * @brief Generic wrapper for a Video Output device.
 */
class VideoOutputDevice {
    public:
        /** Standard Constructor */
        VideoOutputDevice();

        /** @brief Initializes the output device and sets the m_ok flag.
         *
         *  @note This is a wrapper for the InitializeOutput() virtual method.
         *  @return True on success; false otherwise.
         */
        bool Init();

         /** Returns the OK status for the output device. */
        bool IsOk();

        /** @brief Clears and disconnects the output device. Sets m_ok to false.
          *
          * @note This method can be called ONLY by the main thread!
          */
        void ShutDown();

        /** Returns the output color format. */
        VideoColorFormat GetColorFormat();

        /** Returns the output width, in pixels. */
        unsigned int GetWidth();

        /** Returns the output height, in pixels. */
        unsigned int GetHeight();

        /** @brief Called whenever the output screen size is changed (i.e. by resizing the playback window)
         *
         *  @param newwidth the new width to be set for playback
         *  @param newheight the new height to be set for playback
         *  @return true if size was successfully changed, false otherwise.
         *  @note This is a wrapper for the protected method ChangeDeviceSize(). After change takes place, variables
         *  m_width and m_height are updated.
         *  @note This method can be called ONLY by the main thread!
         */
        bool ChangeSize(unsigned int newwidth,unsigned int newheight);

        /** @brief Loads video data from an external buffer.
         *
         *  @param colorformat The input color format being sent.
         *  @param buf Buffer containing the data to be processed.
         *  @param buflen The length of the buffer to be processed, in bytes.
         *  @note  This method is a wrapper for LoadDeviceVideoData.
         *  @note  This method should be called by the worker thread
         */
        void LoadVideoData(VideoColorFormat colorformat, const char *buf,unsigned int buflen);

        /** Standard destructor. */
        virtual ~VideoOutputDevice();


    protected:

        /** @brief Initializes the output device.
          *
          * @note The m_ok flag is set to this method's return value.
          * @return True on success; false otherwise.
          * @note This method MUST set the m_width, m_height and m_colorformat variables.
          */
        virtual bool InitializeOutput();

        /** Clears the video output device (i.e. sets it all to black). Called by ShutDown(). */
        virtual void Clear();

        /** Disconnects the video output device or memory resource. Called by ShutDown(). */
        virtual void DisconnectOutput();

        /** @brief Flag indicating that playback must be aborted immediately.
          * @return true if playback/encoding thread must be aborted; false otherwise.
          * @note This method MUST be called by LoadDeviceVideoData.
          */
        bool MustAbortPlayback();

        /** @brief Called by ChangeSize whenever the output screen size is changed.
          *
          * This method must deal with size changes so that it resizes the buffers used for playback.
          * @param newwidth the new width to be set for playback
          * @param newheight the new height to be set for playback
          * @return true if size was changed, false otherwise.
          */
        virtual bool ChangeDeviceSize(unsigned int newwidth,unsigned int newheight);

        /** @brief Virtual method which loads video data from an external buffer.
         *
         *  @param colorformat The input color format being sent.
         *  @param buf Buffer containing the data to be processed.
         *  @param buflen The length of the buffer to be processed, in bytes.
         *  @note  This method MUST do nothing if either m_width or m_height are set to 0.
         *         When the data is finally converted, RenderData MUST be called.
         *  @note  This method MUST check MustAbortPlayback() regularly and abort rendering when the result is true.
         */
        virtual void LoadDeviceVideoData(VideoColorFormat colorformat, const char *buf,unsigned int buflen);


        /** Plays the received frames.
          * @note This method MUST check MustAbortPlayback() regularly and abort rendering when the result is true.
          * @note This method MUST do nothing if either m_width or m_height are set to 0.
          */
        virtual void RenderData();

        /** Output color format */
        VideoColorFormat m_colorformat;

        /** Output width */
        unsigned int m_width;

        /**  Output height */
        unsigned int m_height;

    private:

        /** flag that indicates that playing is ACTUALLY taking place. */
        bool m_playing;

        /** flag that forbids playback when changing the device size */
        bool m_changingsize;

        /** flag that forbids playback when shutting down the device */
        bool m_shuttingdown;

        bool m_ok;                      /** Tells whether the output device was initialized correctly. */
        syMutex* m_mutex;
};

#endif
