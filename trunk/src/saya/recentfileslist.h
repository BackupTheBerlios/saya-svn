/***************************************************************
 * Name:      recentfileslist.h
 * Purpose:   Declaration of a Recent Files list class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef recentfileslist_h
#define recentfileslist_h

class syString;

/** RecentFilesList implements a double-ended string queue
 *  that handles a recently opened files list. Items added
 *  are checked for duplicates before inclusion. Also,
 *  it includes a counter to catch any changes in the list.
 */
class RecentFilesList {
    public:

        /** Standard constructor */
        RecentFilesList(unsigned int capacity);

        /** Standard destructor */
        ~RecentFilesList();

        /** Prepends a file to the beginning of the list. */
        void Add(const char* s, bool fromthebeginning = true);

        /** Clears the list. */
        void clear();

        /** Gets an item from the list
         *  @note The function is one-based.
         */
        const syString item(unsigned int i) const;

        /** Gets the current list size. */
        unsigned int size() const;

        /** Gets the counter for refreshing */
        unsigned int Counter() const;

        /** Updates a destination counter, and if it's different than the current counter, returns true. */
        bool UpdateCounter(unsigned int& destinationcounter);

    private:
        class Data;
        Data* m_Data;
};

#endif
