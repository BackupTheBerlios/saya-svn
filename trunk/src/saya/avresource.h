/***************************************************************
 * Name:      avresouce.h
 * Purpose:   Declaration for the AVResource class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef avresource_h
#define avresource_h

#include "serializable.h"

class AVSettings;

enum ResourceType   /** Used for the video resources. */
{
    RTSequence = 0, /**<Enum value RTSequence. */
    RTVideoFile,    /**<Enum value RTVideoFile. */
    RTAudioFile,    /**<Enum value RTAudioFile. */
    RTImageFile,    /**<Enum value RTImageFile. */
    RTOfflineFile,  /**<Enum value RTOfflineFile. */
    RTTitle,        /**<Enum value RTTitle. */
    RTBarsAndTone,  /**<Enum value RTBarsAndTone. */
    RTBlackVideo,   /**<Enum value RTBlackVideo. */
    RTColorMatte,   /**<Enum value RTColorMatte. */
    RTUCLeader      /**<Enum value RTUCLeader. */
};

class AVResource:public serializable {
    public:
        /** Standard constructor. */
        AVResource();

        /** Standard destructor. */
        virtual ~AVResource();

        /** The Resource id. */
        unsigned int m_ResourceId;

        /** The resource type. @see ResourceType */
        ResourceType m_ResourceType;

        /** @brief The filename (if applicable) used by the resource.
          *
          * This is the resource's full pathname. It is calculated on
          * startup based on m_RelativeFilename and the project's
          * path.
          */
        std::string m_Filename;

        /** @brief The relative filename for the resource.
          *
          * Resources are stored in a directory below the project's path.
          * This way they can easily be located and transported.
          */
        std::string m_RelativeFilename;

        /** @brief Resource's icon.
          *
          * The icon is stored in 64x64 JPEG, encoded in base64.
          */
        std::string m_Icon; // 64x64 JPEG icon encoded with base64

        /** @brief Video Settings for the clip.
          *
          * @see AVSettings
          */
        AVSettings* m_AVSettings;

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();
};

#endif
