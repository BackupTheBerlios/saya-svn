/***************************************************************
 * Name:      avcontroller.h
 * Purpose:   Declaration for the AVController class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef avcontroller_h
#define avcontroller_h

#include "avtypes.h"

class AVSource;
class VideoOutputDevice;
class AudioOutputDevice;
class AVControllerData;

class AVController {
    public:

        /** Standard constructor */
        AVController();

        /** Standard destructor */
        virtual ~AVController();

        /** @brief Sets the Video Input Device. Must be called before Init().
         *  @param device The video input.
         *  @return true on success; false if playing or if video-in is reserved.
         *  @warning This function must be called by the main thread ONLY!
         */
        bool SetVideoIn(AVSource* device);

        /** @brief Sets the Audio Input Device. Must be called before Init().
         *  @param device The audio input.
         *  @return true on success; false if playing or if audio-in is reserved.
         *  @warning This function must be called by the main thread ONLY!
         */
        bool SetAudioIn(AVSource* device);

        /** @brief Sets the Video Output Device. Must be called before Init().
         *  @param device The video output.
         *  @return true on success; false if playing or if video-out is reserved.
         *  @warning This function must be called by the main thread ONLY!
         */
        bool SetVideoOut(VideoOutputDevice* device);

        /** @brief Sets the Audio Output Device. Must be called before Init().
         *  @param device The audio output.
         *  @return true on success; false if playing or if audio-out is reserved.
         *  @warning This function must be called by the main thread ONLY!
         */
        bool SetAudioOut(AudioOutputDevice* device);

        /** @brief Initializer. Sets the new A/V devices, shutting down previously if necessary.
         *  @warning This function must be called by the main thread ONLY!
         */
        void Init();

        /** @brief Quits all threads and shuts down all devices.
         *
         *  @note Also called by Init.
         *  @warning Devices are not released from memory. You must do that yourself.
         *  @warning This function must be called by the main thread ONLY!
         */
        void ShutDown();

        /** Sets the playback video framerate.
         *
         *  @note This is only for playback, not encoding.
         */
        void SetPlaybackFramerate(float framerate);

        /** @brief Plays video at given speed for the given duration.
         *
         *  @param speed Desired playback speed (can be negative). 1.0 = normal. If 0, playback is paused.
         *  @param duration Playback duration, in nanoseconds. 0 = Unlimited.
         *  @param muted Whether playback should be video-only (default false).
         *  @note  Only valid for non-encoding output devices.
         */
        void Play(float speed = 1.0, avtime_t duration = 0,bool muted = false);

        /** @brief Plays only video (not audio) at given speed for the given duration.
         *
         *  @param speed Desired playback speed (can be negative). 1.0 = normal. If 0, playback is paused.
         *  @param duration Playback duration, in nanoseconds. 0 = Unlimited.
         *  @note  Only valid for non-encoding VideoOutputDevices.
         */
        void PlayVideo(float speed = 1.0, avtime_t duration = 0);

        /** @brief Plays only audio (not video) at given speed for the given duration.
         *
         *  @param speed Desired playback speed (can be negative). 1.0 = normal. If 0, playback is paused.
         *  @param duration Playback duration, in nanoseconds. 0 = Unlimited.
         *  @note  Only valid for non-encoding AudioOutputDevices.
         */
        void PlayAudio(float speed = 1.0, avtime_t duration = 0);

        /** @brief Encodes video and audio at given speed for the given duration.
         *
         *  @param speed Desired playback speed (can be negative). 1.0 = normal. If 0, no output is sent.
         *  @param duration Duration of the data stream, in nanoseconds. 0 = Unlimited.
         *  @param muted Whether we should encode video only (default false).
         *  @note  This function is only valid for non-encoding output devices.
         */
        void Encode(float speed = 1.0, avtime_t duration = 0,bool muted = false);

        /** @brief Encodes only video (not audio) at given speed for the given duration.
         *
         *  @param speed Desired playback speed (can be negative). 1.0 = normal. If 0, no output is sent.
         *  @param duration Duration of the data stream, in nanoseconds. 0 = Unlimited.
         *  @note  This function is only valid for non-encoding output devices.
         */
        void EncodeVideo(float speed = 1.0, avtime_t duration = 0);

        /** @brief Plays only audio (not video) at given speed for the given duration.
         *
         *  @param speed Desired playback speed (can be negative). 1.0 = normal. If 0, no output is sent.
         *  @param duration Duration of the data stream, in nanoseconds. 0 = Unlimited.
         *  @note  This function is only valid for non-encoding output devices.
         */
        void EncodeAudio(float speed = 1.0, avtime_t duration = 0);

        /** @brief Sends a snapshot of the current frame to the VideoOutputDevice.
         *
         *  @note  Only valid for non-encoding VideoOutputDevices.
         */
        void Snapshot();

        /** @brief Disables video frame skipping on Playback. Takes effect on the next play. */
        void DontSkipVideoFrames(bool dontskip);

        /** Gets the status of non-frame-skipping flag. */
        bool GetDontSkipVideoFrames();

        /** Pauses playback / encoding. */
        void Pause();

        /** @brief Stops playback / encoding.
         *
         *  @note Also called by ShutDown().
         */
        void Stop();

        /** @brief Gets the video frame corresponding to the given time.
         *  @param time Instant in time where we want to get the frame index.
         *  @return The frame index (zero-based) corresponding to the given time.
         *  @note This is a wrapper for VideoInputDevice::GetFrameIndex().
         */
        unsigned long GetVideoFrameIndex(avtime_t time);

        /** @brief Gets the time corresponding to a given frame.
         *  @param The frame index (zero-based) where we want to get the time.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The instant in time from the video start corresponding to the given frame.
         *  @note This is a wrapper for VideoInputDevice::GetTimeFromFrameIndex().
         */
        avtime_t GetTimeFromVideoFrameIndex(unsigned long  frame, bool fromend = false);

        /** @brief Gets the video current time.
         *  In theory it should be the same as the audio, unless they were seeked with different times.
         */
        avtime_t GetCurrentVideoTime();

        /** @brief Gets the audio current time.
         *  In theory it should be the same as the video, unless they were seeked with different times.
         */
        avtime_t GetCurrentAudioTime();

        /** Gets the video frame that is currently being played. */
        unsigned long GetCurrentVideoFrame();

        /** @brief Seeks the Input to a given point in time.
         *
         *  @param time The time in nanoseconds to jump to.
         *  @param fromend Are we seeking from the end of the stream?
         *  @return the resulting position in time.
         *  @note  For playback, playback will be paused.
         *  @warning During an encoding operation, all seeks are invalid.
         */
        avtime_t Seek(avtime_t time,bool fromend = false);

        /** @brief Seeks the Input Video to a given point in time.
         *
         *  @param time The time in nanoseconds to jump to.
         *  @param fromend Are we seeking from the end of the stream?
         *  @return the resulting position in time.
         *  @note  For playback, playback will be paused.
         *  @warning During an encoding operation, all seeks are invalid.
         */
        avtime_t SeekVideo(avtime_t time,bool fromend = false);

        /** @brief Seeks the Input Audio to a given point in time.
         *
         *  @param time The time in nanoseconds to jump to.
         *  @param fromend Are we seeking from the end of the stream?
         *  @return the resulting position in time.
         *  @note  For playback, playback will be paused.
         *  @warning During an encoding operation, all seeks are invalid.
         */
        avtime_t SeekAudio(avtime_t time,bool fromend = false);

        /** @brief Seeks to the time corresponding to a determinate video frame.
         *
         *  @param frame The frame to seek to.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         */
        avtime_t SeekFrame(unsigned long frame,bool fromend = false);

        /** @brief Seeks video only to a determinate video frame.
         *
         *  @param frame The frame to seek to.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         */
        avtime_t SeekVideoFrame(unsigned long frame,bool fromend = false);

        /** Seeks to the time corresponding to a relative video frame (positive fast forwards, negative rewinds) */
        avtime_t SeekFrameRelative(long frame);

        /** Seeks video only to the time corresponding to a relative video frame (positive fast forwards, negative rewinds) */
        avtime_t SeekVideoFrameRelative(long frame);

        /** @brief Gets the input length in nanoseconds.
         *
         *  @note If the video and audio are of different lengths, the greatest is used.
         */
        avtime_t GetLength();

        /** Gets the video length in nanoseconds. */
        avtime_t GetVideoLength();

        /** Gets the audio length in nanoseconds. */
        avtime_t GetAudioLength();

        /** @brief Returns the end-of-file status for the input.
         *
         *  @note If video and audio are of different lengths, the greatest length is used for the calculation.
         */
        bool IsEof();

        /** Returns the end-of-file status for the video input. */
        bool IsVideoEof();

        /** Returns the end-of-file status for the audio input. */
        bool IsAudioEof();

        /** @brief Gets the Video Output device's IsEncoder() flag.
         *
         *  @return true if the Video Output device is an encoder; false otherwise.
         */
         bool IsVideoEncoder();

        /** @brief Gets the Audio Output device's IsEncoder() flag.
         *
         *  @return true if the Audio Output device is an encoder; false otherwise.
         */
         bool IsAudioEncoder();

        /** @brief Gets the Video and Audio Output device's IsEncoder() flag.
         *
         *  @return true if either of the Audio or Video output devices is an encoder; false otherwise.
         */
         bool IsEncoder();

        /** @brief Checks if all of the Output devices are Encoders or NULL.
         *
         *  @return true if both of the Audio or Video output devices are encoders or NULL; false otherwise.
         */
         bool IsEncoderOnly();


        /** @brief Gets the playback / encoding status.
         *
         *  @return True if currently playing / encoding, false otherwise.
         *  @note   If playback / encoding is paused, it will return false.
         */
         bool IsPlaying();

        /** Thread Id for Audio In */
        unsigned long GetAudioInThreadId();

        /** Thread Id for Video In */
        unsigned long GetVideoInThreadId();

        /** Thread Id for Audio Out */
        unsigned long GetAudioOutThreadId();

        /** Thread Id for Video Out */
        unsigned long GetVideoOutThreadId();

        /** Sets the maximum framerate, in frames per second. */
        static void SetMaximumFrameRate(float maxframerate);

    protected:

        /** Initializes the devices */
        void InitDevices(AVSource* videoin,AVSource* audioin, VideoOutputDevice* videoout,AudioOutputDevice* audioout);

        /** Sets the Video input device. */
        bool InnerSetVideoIn(AVSource* device);
        /** Sets the Video output device. */
        bool InnerSetVideoOut(VideoOutputDevice* device);
        /** Sets the Audio input device. */
        bool InnerSetAudioIn(AVSource* device);
        /** Sets the Audio output device. */
        bool InnerSetAudioOut(AudioOutputDevice* device);

        /** @brief Flag indicating that playback must pause.
         *
         *  @return true if we must pause; false otherwise.
         */
        bool MustPause();

        /** @brief Flag indicating that playback must stop.
         *
         *  @return true if we must stop; false otherwise.
         */
        bool MustStop();

        /** @brief Sets the minimum quantity of data (in milliseconds) that will be sent
         *  to the AudioOutputDevice on each iteration.
         *
         *  If you set it too short, you may experience skipping.
         *  On the other hand, if you set it too long, playback may become unresponsive.
         *  Recommended is 100ms (default).
         *  @note Value cannot be changed during playback.
         */
        void SetAudioGranularity(unsigned long granularity);

        /** @brief Set this flag to true if you're using an internal video input device. */
        bool m_ReservedVideoIn;
        /** @brief Set this flag to true if you're using a fixed audio input device. */
        bool m_ReservedAudioIn;
        /** @brief Set this flag to true if you're using a fixed video output device. */
        bool m_ReservedVideoOut;
        /** @brief Set this flag to true if you're using a fixed audio output device. */
        bool m_ReservedAudioOut;

    private:
        AVControllerData* m_Data;
};

#endif
