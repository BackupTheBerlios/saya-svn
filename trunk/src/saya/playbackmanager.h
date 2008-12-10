/***************************************************************
 * Name:      playbackmanager.h
 * Purpose:   Declaration for the Playback Manager class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-12-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef playback_manager_h
#define playback_manager_h

class AVController;

class PlaybackManager {
    public:
        PlaybackManager();
        ~PlaybackManager();

        AVController* GetInputMonitor() const;
        AVController* GetPreviewMonitor() const;

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};


#endif
