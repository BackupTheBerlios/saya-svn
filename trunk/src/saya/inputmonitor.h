/***************************************************************
 * Name:      inputmonitor.h
 * Purpose:   Declaration for the InputMonitor class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef inputmonitor_h
#define inputmonitor_h

class InputMonitorData;
class syString;

#include "core/avcontroller.h"
class InputMonitor : public AVController {
    friend class InputMonitorData;

    public:

        /** Default constructor. */
        InputMonitor();

        /** Destructor. */
        virtual ~InputMonitor();

        /** @brief Sets the file to read.
         *  @param filename The file to be read.
         *  @return true on success; false if the file couldn't be set due to playback.
         *  @warning This function must be called ONLY by the main thread.
         */
        bool SetFile(syString filename);

        const syString GetFile() const;

        void Init(VideoOutputDevice* videoout, AudioOutputDevice* audioout);

    private:
        InputMonitorData* m_Data;
};

#endif
