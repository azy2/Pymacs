#ifndef SDL_BUFFER_H
#define SDL_BUFFER_H

#include <cstdint>
#include <boost/container/vector.hpp>
#include <ext/rope>
#include <boost/container/string.hpp>
#include <fstream>
#include "Viewport.h"

using namespace __gnu_cxx;
using namespace boost::container;

class Buffer {
public:
    Buffer(int fontWidth, int fontHeight, Viewport viewport = Viewport(0, 0, 0, 0));
    Buffer(std::string filePath, int fontWidth, int fontHeight, Viewport viewport = Viewport(0, 0, 0, 0));
    ~Buffer();
    void insert(uint8_t c);

    void remove();
    uint64_t size();
    std::string getLine(uint64_t line);
    const char* c_str();

    uint64_t index();
    uint64_t lineIndex();

    void moveCursorForward();
    void moveCursorBackward();
    void moveCursorUp();
    void moveCursorDown();

    uint64_t cursorLineIndex();
    uint64_t lineStartIndex(uint64_t l);
    uint64_t lineEndIndex(uint64_t l);
    uint64_t lineLength(uint64_t l);

    void updateViewport(Viewport viewport);

    vector<crope::iterator> *lines;
    Viewport viewport = Viewport(0, 0, 0, 0);
    int relativeLineToDisplayCursor;
    int linesPerScreen;
    int partialLines;
private:
    crope *data;
    crope::iterator cursor;
    std::fstream fileHandle;
    vector<crope::iterator>::iterator line;

    int fontWidth;
    int fontHeight;

    uint64_t previousCursorDistance;
    void calculateLinesPerScreen();
};


#endif //SDL_BUFFER_H
