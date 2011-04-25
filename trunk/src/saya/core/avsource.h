/***************************************************************
 * Name:      avsource.h
 * Purpose:   Declaration for the AVSource class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef avsource_h
#define avsource_h

#include "videocolorformat.h"
#include "avdevice.h"
#include "avtypes.h"

class sySafeMutex;
class syBitmap;
class syBitmapSink;
class syAudioBuffer;
class AudioOutputDevice;
class AVSource;

typedef AVSource* (*AVSourceFactoryFunction)();

/**
 * AVSource is the base class for video and audio providers.
 * The functions you must override are GetFrameIndex(), LoadCurrentFrame() and SeekResource().
 * The functions AllocateResources and FreeResources MUST call the parent class' methods.
 */
class AVSource : public AVDevice {

    public:

        /** Standard constructor. */
        AVSource();

        /** Standard destructor. */
        virtual ~AVSource();

        /** @brief Seeks to a determinate video frame.
         *
         *  @param frame The frame to seek to.
         *  @param fromend Boolean telling the source to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         *  @note  This is a thread-safe wrapper for the protected functions InternalSeek() and SeekResource().
         */
        avtime_t SeekVideoFrame(unsigned long frame,bool fromend = false);

        /** @brief Seeks to a determinate instant in time.
         *
         *  @param time The time, in milliseconds, to seek to.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         *  @note  This is a thread-safe wrapper for the protected functions InternalSeek() and SeekResource().
         */
        avtime_t SeekVideo(avtime_t time,bool fromend = false);

        /** @brief Seeks to a determinate instant in time.
         *
         *  @param time The time, in milliseconds, to seek to.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         *  @note  This is a thread-safe wrapper for the protected functions InternalSeek() and SeekResource().
         */
        avtime_t SeekAudio(avtime_t time,bool fromend = false);

        /** Gets the current video position in time */
        avtime_t GetVideoPos() const;

        /** Gets the current video position in time */
        avtime_t GetAudioPos() const;

        /** @brief Gets the length (time) of the video data being read.
         *  @return The length of the resource's video data, in avtime_t units. Minimum one.
         */
        avtime_t GetVideoLength() const;

        /** @brief Gets the length (time) of the audio data being read.
         *  @return The length of the resource's audio data, in avtime_t units. Minimum one.
         */
        avtime_t GetAudioLength() const;

        /** @brief Gets the length (time) of the data being read (both video and audio).
         *  @return The length of the source's data, in avtime_t units. Minimum one.
         */
        avtime_t GetLength() const;

        /** Gets the Video Color format of the resource for Video sources. */
        VideoColorFormat GetColorFormat() const;

        /** Gets the video source's width in pixels.  */
        unsigned long GetWidth() const;

        /** Gets the video source's height in pixels. */
        unsigned long GetHeight() const;

        /** Gets the video source's pixel aspect ratio. */
        float GetPixelAspect() const;

        /** Gets the video source's framerate. */
        float GetFramesPerSecond() const;

        /** @brief Sends the current video frame to the specified syBitmapSink.
         *
         * This routine just calls LoadCurrentFrame() and then
         * calls sink->LoadVideoData(this->m_Bitmap).
         */
        void SendCurrentFrame(syBitmapSink* sink);

        /** @brief Gets the video frame corresponding to the given time.
         *  @param time Instant in time where we want to get the frame index.
         *  @return The frame index (zero-based) corresponding to the given time.
         *  @note The calculation is done assumming a constant input framerate.
         *  For variable framerate, you must override this method.
         */
        virtual unsigned long GetFrameIndex(avtime_t time);

        /** @brief Gets the time corresponding to a given video frame.
         *  @param The frame index (zero-based) where we want to get the time.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The instant in time from the video start corresponding to the given frame.
         *  @note The calculation is done assumming a constant input framerate.
         *  For variable framerate, you must override this method.
         */
        virtual avtime_t GetTimeFromFrameIndex(unsigned long  frame, bool fromend = false);

        /** @brief Registers an AVSource factory with a specific URL.
         *  @param url The string to register the factory with.
         *  @param func The factory function to register.
         *  @return true always; The return value was added to help initializer functions.
         */
        static bool RegisterSource(const char* url, AVSourceFactoryFunction func);

        /** @brief Unregisters a Video Input Device factory with a specific URL.
         *  @param url The string to unregister the factory.
         */
        static void UnregisterSource(const char* url);

        /** @brief Creates a VideoInputDevice registered with a specific URL. */
        static AVSource* CreateSource(const char* url);

        /** Returns the currently used bitmap. */
        virtual const syBitmap* GetBitmap();

        /** @brief Gets the audio sample number corresponding to the given time.
         *  @param time Instant in time (milliseconds) where we want to get the sample index.
         *  @return The sample index (zero-based) corresponding to the given time.
         */
        virtual unsigned long GetSampleIndex(avtime_t time);

        /** @brief Sends the buffer contents to the specified AudioOutputDevice.
         *
         * This method just calls LoadAudioBuffer() and then
         * calls device->LoadAudioData(this->m_Buffer, numsamples).
         * @param device the AudioOutputDevice object to send the data to.
         * @param numsamples The number of samples to send. 0 = unlimited.
         * @note When sending data, the device will not exit until the specified number
         * of samples has been sent, we have reached EOF, or a stop/abort signal has been received.
         */
        void SendAudioData(AudioOutputDevice* device,unsigned long numsamples = 0);

        /** @brief Sends the buffer contents to the specified AudioOutputDevice.
         *
         *  This method calls LoadAudioBuffer() and then device->LoadAudioData.
         *  If duration != 0, the number of samples is obtained via GetSampleIndex;
         *  then SendAudioData is called using the obtained number of samples.
         */
        void SendAudioData(AudioOutputDevice* device,avtime_t duration = 0);

    protected:

        /** @brief Loads the current frame into m_Bitmap.
         *
         *  This is a stub; you need to override this function to acomplish anything.
         *  @warning You MUST NOT call Seek() from LoadCurrentFrame(), or you will trigger a mutex deadlock!!
         *  If you need to do a seeking, call InternalSeek() instead.
         */
        virtual void LoadCurrentFrame();

        /** @brief Loads a chunk of audio into m_Buffer.
         *
         *  @param numsamples The maximum number of samples to be read. 0 = let the derived class choose.
         *  This is a stub; you need to override this function to acomplish anything.
         *  @warning You MUST NOT call Seek() from LoadAudioBuffer(), or you will trigger a mutex deadlock!!
         *  If you need to do a seeking, call InternalSeek() instead.
         *  @note This method MUST NOT exit until the specified number of samples has been sent,  we
         *  have reached EOF, OR a stop/abort signal has been received.
         *
         */
        virtual void LoadAudioBuffer(unsigned long numsamples = 0);

        /** @brief Internal video seeking routine.
         *
         *  @param time The time, in avtime_t units, to seek to.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         *  @note Called by Seek(); Calls SeekResource().
         */
        avtime_t InternalVideoSeek(avtime_t time, bool fromend = false);

        /** @brief Internal audio seeking routine.
         *
         *  @param time The time, in avtime_t units, to seek to.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         *  @note Called by Seek(); Calls SeekResource().
         */
        avtime_t InternalAudioSeek(avtime_t time, bool fromend = false);

        /** @brief Seeks to a determinate instant in time.
         *
         *  @param time The time, in avtime_t units, to seek to.
         *  @return The current instant in time where the current frame will be read.
         *  @note  This function is called by InternalVideoSeek().
         *  @warning This function MUST NOT update m_CurrentVideoTime. That is done by InternalVideoSeek().
         */
        virtual avtime_t SeekVideoResource(avtime_t time) { return time; }

        /** @brief Seeks to a determinate instant in time.
         *
         *  @param time The time, in avtime_t units, to seek to.
         *  @return The current instant in time where the current frame will be read.
         *  @note  This function is called by InternalSeek().
         *  @warning This function MUST NOT update m_CurrentAudioTime. That is done by InternalAudioSeek().
         */
        virtual avtime_t SeekAudioResource(avtime_t time) { return time; }

        /** @brief Allocates memory for the Bitmap. Called by Init().
         *  @note If you override this function, remember to call it in your derived class' AllocateResources()
         */
        virtual bool AllocateResources();

        /** @brief Releases the memory for the Bitmap and Audio buffers. Called by ShutDown().
         *  @note If you override this function, remember to call it in your derived class' FreeResources().
         */
        virtual void FreeResources();

        /** @brief The bitmap for the current video frame.
         *
         *  @note This member is protected because the only authorized way to send
         *  the info to an external object is through the SendCurrentFrame functions.
         *  This way you can implement a cache of most used frames or something.
         */
        syBitmap* m_Bitmap;

        /** Circular Buffer to hold the audio data */
        syAudioBuffer* m_AudioBuffer;

        /** @brief A pointer indicating the current video time in the resource.
         *
         *  Modified by SeekVideo().
         */
        avtime_t m_CurrentVideoTime;

        /** @brief A pointer indicating the current video time in the resource.
         *
         *  Modified by SeekAudio().
         */
        avtime_t m_CurrentAudioTime;

        /** Indicates the resource's total video length in avtime_t units. Minimum one. */
        avtime_t m_VideoLength;

        /** Indicates the resource's total video length in avtime_t units. Minimum one. */
        avtime_t m_AudioLength;

        /** The width of the current resource. */
        unsigned long m_Width;

        /** The height of the current resource. */
        unsigned long m_Height;

        /** The Video Color format of the current resource. */
        VideoColorFormat m_ColorFormat;

        /** The pixel aspect ratio for the current resource. */
        float m_PixelAspect;

        /** The fps indicator, if any. Default = 0. */
        float m_FramesPerSecond;

        /** The Number of Audio Channels to reserve. */
        unsigned int m_NumAudioChannels;

        /** The Sample Frequency used for the audio. */
        unsigned int m_AudioFrequency;

        /** The Precision (number of bits) used for the audio. */
        unsigned int m_AudioPrecision;

        /** The default buffer size (in samples) used for the audio. */
        unsigned int m_AudioBufferSize;
};

#endif
