#ifndef PYMACS_EDITOR_H
#define PYMACS_EDITOR_H

#include "PlatformLayer.h"
#include "Buffer.h"

class Editor {
public:
    Editor(PlatformLayer *platformLayer);
    void loop();

private:
    PlatformLayer *platformLayer;
    std::vector<Buffer*> visibleBuffers;
    std::vector<Buffer*> invisibleBuffers;
    std::vector<Buffer*>::iterator activeBuffer;
};

#endif //PYMACS_EDITOR_H
