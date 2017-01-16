#include <iostream>
#include "Buffer.h"

Buffer::Buffer(int fontWidth, int fontHeight, Viewport viewport) {
    this->fontWidth = fontWidth;
    this->fontHeight = fontHeight;
    this->viewport = viewport;

    data = new crope("\n");
    lines = new vector<crope::iterator>;
    lines->push_back(data->mutable_begin());
    line = lines->begin();

    cursor = data->mutable_begin();
    previousCursorDistance = 0;
    calculateLinesPerScreen();
    relativeLineToDisplayCursor = 0;
}

Buffer::Buffer(std::string filePath, int fontWidth, int fontHeight, Viewport viewport) {
    this->fontWidth = fontWidth;
    this->fontHeight = fontHeight;
    this->viewport = viewport;

    fileHandle.open(filePath, std::ios::in | std::ios::binary | std::ios::ate);
    // TODO: Don't load entire file? Dynamically read the parts needed to edit?
    // TODO: Warn about large file sizes? Refuse if not enough memory or just let the user suffer the consequences?
    if (fileHandle.is_open()) {
        data = new crope();
        lines = new vector<crope::iterator>;
        std::streampos fileSize = fileHandle.tellg();
        fileHandle.seekg(0, std::ios::beg);
        char mem[1];
        cursor = data->mutable_begin();

        for (int i = 0; i < fileSize; i++) {
            fileHandle.read(mem, 1);
            cursor = data->insert(cursor, mem[0]);
            ++cursor;

            if (mem[0] == '\n') {
                lines->push_back(data->mutable_end() - 1);
            }
        }

        cursor = data->mutable_begin();
        line = lines->begin();
        previousCursorDistance = 0;
        calculateLinesPerScreen();
        relativeLineToDisplayCursor = 0;
    } else {
        //TODO: Better error handling/logging?
        throw new std::runtime_error("Could not open file, proceeding with default constructor");
    }
}

Buffer::~Buffer() {
    delete data;
    delete lines;

    if (fileHandle.is_open()) {
        fileHandle.close();
    }
}

void Buffer::calculateLinesPerScreen() {
    float lps = (float)viewport.h / fontHeight;
    if (lps == ceil(lps)) {
        partialLines = 0;
    } else {
        partialLines = 1;
    }
    linesPerScreen = (int)ceil(lps);
}

uint64_t Buffer::index() {
    return cursor.index();
}

uint64_t Buffer::lineIndex() {
    return lines->index_of(line);
}

void Buffer::insert(uint8_t c) {
    cursor = data->insert(cursor, c);
    if (c == '\n') {
        line = lines->insert(line, cursor);
        ++line;
    }
    ++cursor;

    for (vector<crope::iterator>::iterator it = line; it != lines->end(); ++it) {
        ++(*it);
    }
}

void Buffer::remove() {
    if (cursor != data->mutable_begin()) {
        --cursor;

        if (*cursor == '\n') {
            --line;
            line = lines->erase(line);
        }

        cursor = data->erase(cursor);

        for (vector<crope::iterator>::iterator it = line; it != lines->end(); ++it) {
            --(*it);
        }
    }

}

uint64_t Buffer::size() {
    return data->size();
}

uint64_t Buffer::cursorLineIndex() {
    return cursor.index() - lineStartIndex(lines->index_of(line));
}

uint64_t Buffer::lineStartIndex(uint64_t l) {
    if (l == 0) {
        return 0;
    }
    return lines->at(l - 1).index() + 1;
}

uint64_t Buffer::lineEndIndex(uint64_t l) {
    return lines->at(l).index();
}

uint64_t Buffer::lineLength(uint64_t l) {
    return lineEndIndex(l) - lineStartIndex(l);
}

void Buffer::moveCursorForward() {
    if (cursor != data->mutable_end() - 1) {
        if (*cursor == '\n') {
            ++line;
            if (relativeLineToDisplayCursor < linesPerScreen - 1 - partialLines) {
                ++relativeLineToDisplayCursor;
            }
        }
        ++cursor;
        previousCursorDistance = 0;
    }
}

void Buffer::moveCursorBackward() {
    if (cursor != data->mutable_begin()) {
        --cursor;
        if (*cursor == '\n') {
            --line;
            if (relativeLineToDisplayCursor > 0) {
                --relativeLineToDisplayCursor;
            }
        }
        previousCursorDistance = 0;
    }
}

void Buffer::moveCursorUp() {
    if (lines->index_of(line) > 0) {
        uint64_t distance;
        if (previousCursorDistance) {
            distance = previousCursorDistance;
            previousCursorDistance = 0;
        } else {
            distance = cursorLineIndex();
        }

        // Cursor can stay in the same column, because the previous line is at least as long as the cursor pos
        if (distance <= lineLength(lines->index_of(line) - 1)) {
            --line;
            if (lines->index_of(line) == 0) {
                cursor = data->mutable_begin() + distance;
            } else {
                cursor = *(line - 1) + distance + 1;
            }
        }
            // We have to put the cursor on the end of the line because the line is too short.
        else {
            --line;
            cursor = *line;
            previousCursorDistance = distance;
        }

        if (relativeLineToDisplayCursor > 0) {
            --relativeLineToDisplayCursor;
        }
    } else {
        cursor = data->mutable_begin();
        line = lines->begin();
    }
}

void Buffer::moveCursorDown() {
    if (lines->index_of(line) < lines->size() - 1) {
        uint64_t distance;
        if (previousCursorDistance) {
            distance = previousCursorDistance;
            previousCursorDistance = 0;
        } else {
            distance = cursorLineIndex();
        }

        // Cursor can stay in the same column, because the previous line is at least as long as the cursor pos
        if (distance <= lineLength(lines->index_of(line) + 1)) {
            ++line;
            cursor = *(line - 1) + distance + 1;
        }
        // We have to put the cursor on the end of the line because the line is too short.
        else {
            ++line;
            cursor = *line;
            previousCursorDistance = distance;
        }

        if (relativeLineToDisplayCursor < linesPerScreen - 1 - partialLines) {
            ++relativeLineToDisplayCursor;
        }
    } else {
        cursor = data->mutable_end() - 1;
        line  = lines->end() - 1;
    }
}

std::string Buffer::getLine(uint64_t line) {
    crope::iterator start;
    if (line == 0) {
        start = data->mutable_begin();
    } else {
        start = lines->at(line - 1);
        ++start;
    }


    crope::iterator end = lines->at(line);

    crope sub = data->substr(start, end);

    return std::string(sub.c_str());
}

const char* Buffer::c_str() {
    return data->c_str();
}

void Buffer::updateViewport(Viewport viewport) {
    this->viewport = viewport;
    calculateLinesPerScreen();
}


