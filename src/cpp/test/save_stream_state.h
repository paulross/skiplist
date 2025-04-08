//
// Created by Paul Ross on 07/04/2025.
//

#ifndef SKIPLIST_SAVE_STREAM_STATE_H
#define SKIPLIST_SAVE_STREAM_STATE_H

#include <iostream>
#include <iomanip>

/**
 * A class that saves the formatting state of a stream and restores it on destruction.
 *
 * Usage:
 *
 * \code
 *  {
 *      StreamFormatState stream_state(os); // Stream state captured.
 *      // ...
 *  } // Stream state restored.
 * \endcode
 *
 * The state that is saved is the \c .flags() and the \c .fill() character.
 */
class StreamFormatState {
public:
    /**
     * Takes a stream and records its formatting state.
     * @param stream The stream.
     */
    StreamFormatState(std::basic_ios<char> &stream) : _pStream(&stream), _ff(stream.flags()), _fill(stream.fill()) {}
    /**
     * Destruction. This restores the formatting state of the stream.
     */
    ~StreamFormatState() {
        if (_pStream) {
            _pStream->flags(_ff);
            _pStream->fill(_fill);
        }
    }
private:
    std::basic_ios<char> *_pStream;
    std::ios_base::fmtflags _ff;
    char _fill;
};

#endif //SKIPLIST_SAVE_STREAM_STATE_H
