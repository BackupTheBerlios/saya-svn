/***************************************************************
 * Name:      avsequence.h
 * Purpose:   Declaration for the Timeline Sequence class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef avsequence_h
#define avsequence_h

#include "serializable.h"

class AVTracks;

class AVSequence: public serializable {
    public:

        /** Standard constructor. */
        AVSequence();

        /** Standard destructor. */
        virtual ~AVSequence();

        std::string m_SeqName; /// User-friendly name for the sequence.

        /** @brief Resource Id for the sequence.
          *
          * Sequences are their own resources, they can be inserted into clips and handled like any video
          * or audio file.
          */

        unsigned int m_ResourceId;

        /** The collection of Video Tracks that belong to the sequence */

        AVTracks* m_VideoTracks;

        /** The collection of Audio Tracks that belong to the sequence */
        AVTracks* m_AudioTracks;

        /** @brief Current cursor position in the timeline.
          *
          * Each sequence has its own cursor, which is used for editing purposes only.
        */
        unsigned int m_CursorPosition;

        /** @brief Sets the Beginning of the Work Area.
          *
          * The Work Area determines which parts of a clip will be rendered.
          * In case of nested sequences, they determine the original length of a clip.
        */
        unsigned int m_BeginWorkArea;

        /** Sets the End of the Work Area. */
        unsigned int m_EndWorkArea;

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();
};

class AVClipboard: public AVSequence {
        AVClipboard() {}

        bool m_Singleclip; /// Set it to indicate it's a single clip you're copying.
        virtual ~AVClipboard() {}
};

#endif
