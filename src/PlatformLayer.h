#ifndef PYMACS_PLATFORMLAYER_H
#define PYMACS_PLATFORMLAYER_H

#include "Buffer.h"
#include "Viewport.h"
#include <queue>
#include <SDL_events.h>

class PlatformLayer {
public:
    virtual void render(std::vector<Buffer*> buffers) = 0;
    virtual Viewport getViewport() = 0;
    virtual void getFontDimensions(int *width, int *height) = 0;
    virtual void startLoop() = 0;
    virtual void finishLoop() = 0;
    // TODO: Create platform independent event container
    virtual std::queue<SDL_Event> getEvents() = 0;
    virtual void handleResize() = 0;
};


#endif //PYMACS_PLATFORMLAYER_H
