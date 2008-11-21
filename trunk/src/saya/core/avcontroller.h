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

class VideoInputDevice;
class VideoOutputDevice;
class AudioInputDevice;
class AudioOutputDevice;
class AVControllerData;

class AVController {
    public:

        /** Standard constructor */
        AVController();

        /** Standard destructor */
        virtual ~AVController();

        /** @brief Sets the Video Input Device. Must be called before Init().
         *  @param videoin The video input.
         *  @return true on success; false if playing or if video-in is reserved.
         *  @warning This function must be called by the main thread ONLY!
         */
        bool SetVideoIn(VideoInputDevice* videoin);

        /** @brief Sets the Audio Input Device. Must be called before Init().
         *  @param audioin The audio input.
         *  @return true on success; false if playing or if audio-in is reserved.
         *  @warning This function must be called by the main thread ONLY!
         */
        bool SetAudioIn(AudioInputDevice* audioin);

        /** @brief Sets the Video Output Device. Must be called before Init().
         *  @param videoout The video output.
         *  @return true on success; false if playing or if video-out is reserved.
         *  @warning This function must be called by the main thread ONLY!
         */
        bool SetVideoOut(VideoOutputDevice* videoout);

        /** @brief Sets the Audio Output Device. Must be called before Init().
         *  @param audioout The audio output.
         *  @return true on success; false if playing or if audio-out is reserved.
         *  @warning This function must be called by the main thread ONLY!
         */
        bool SetAudioOut(AudioOutputDevice* audioout);

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
         *  @param duration Playback duration, in milliseconds. 0 = Unlimited.
         *  @param muted Whether playback should be video-only (default false).
         *  @note  Only valid for non-encoding output devices.
         */
        void Play(float speed = 1.0,unsigned long duration = 0,bool muted = false);

        /** @brief Plays only video (not audio) at given speed for the given duration.
         *
         *  @param speed Desired playback speed (can be negative). 1.0 = normal. If 0, playback is paused.
         *  @param duration Playback duration, in milliseconds. 0 = Unlimited.
         *  @note  Only valid for non-encoding VideoOutputDevices.
         */
        void PlayVideo(float speed = 1.0,unsigned long duration = 0);

        /** @brief Plays only audio (not video) at given speed for the given duration.
         *
         *  @param speed Desired playback speed (can be negative). 1.0 = normal. If 0, playback is paused.
         *  @param duration Playback duration, in milliseconds. 0 = Unlimited.
         *  @note  Only valid for non-encoding AudioOutputDevices.
         */
        void PlayAudio(float speed = 1.0,unsigned long duration = 0);

        /** @brief Encodes video and audio at given speed for the given duration.
         *
         *  @param speed Desired playback speed (can be negative). 1.0 = normal. If 0, no output is sent.
         *  @param duration Duration of the data stream, in milliseconds. 0 = Unlimited.
         *  @param muted Whether we should encode video only (default false).
         *  @note  This function is only valid for non-encoding output devices.
         */
        void Encode(float speed = 1.0,unsigned long duration = 0,bool muted = false);

        /** @brief Encodes only video (not audio) at given speed for the given duration.
         *
         *  @param speed Desired playback speed (can be negative). 1.0 = normal. If 0, no output is sent.
         *  @param duration Duration of the data stream, in milliseconds. 0 = Unlimited.
         *  @note  This function is only valid for non-encoding output devices.
         */
        void EncodeVideo(float speed = 1.0,unsigned long duration = 0);

        /** @brief Plays only audio (not video) at given speed for the given duration.
         *
         *  @param speed Desired playback speed (can be negative). 1.0 = normal. If 0, no output is sent.
         *  @param duration Duration of the data stream, in milliseconds. 0 = Unlimited.
         *  @note  This function is only valid for non-encoding output devices.
         */
        void EncodeAudio(float speed = 1.0,unsigned long duration = 0);

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

        /** @brief Seeks the Input to a given point in time.
         *
         *  @param time The time in milliseconds to jump to.
         *  @param fromend Are we seeking from the end of the stream?
         *  @return the resulting position in time.
         *  @note  For playback, playback will be paused.
         *  @warning During an encoding operation, all seeks are invalid.
         */
        unsigned long Seek(unsigned long time,bool fromend = false);

        /** @brief Seeks the Input Video to a given point in time.
         *
         *  @param time The time in milliseconds to jump to.
         *  @param fromend Are we seeking from the end of the stream?
         *  @return the resulting position in time.
         *  @note  For playback, playback will be paused.
         *  @warning During an encoding operation, all seeks are invalid.
         */
        unsigned long SeekVideo(unsigned long time,bool fromend = false);

        /** @brief Seeks the Input Audio to a given point in time.
         *
         *  @param time The time in milliseconds to jump to.
         *  @param fromend Are we seeking from the end of the stream?
         *  @return the resulting position in time.
         *  @note  For playback, playback will be paused.
         *  @warning During an encoding operation, all seeks are invalid.
         */
        unsigned long SeekAudio(unsigned long time,bool fromend = false);

        /** @brief Gets the input length in milliseconds.
         *
         *  @note If the video and audio are of different lengths, the greatest is used.
         */
        unsigned long GetLength();

        /** Gets the video length in milliseconds. */
        unsigned long GetVideoLength();

        /** Gets the audio length in milliseconds. */
        unsigned long GetAudioLength();

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

    protected:

        /** Initializes the devices */
        void InitDevices(VideoInputDevice* videoin,AudioInputDevice* audioin,
                            VideoOutputDevice* videoout,AudioOutputDevice* audioout);

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
