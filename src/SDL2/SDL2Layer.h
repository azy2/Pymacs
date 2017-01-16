#ifndef PYMACS_SDL2LAYER_H
#define PYMACS_SDL2LAYER_H

#include "../PlatformLayer.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Timer.h"
#include "../Viewport.h"

class SDL2Layer : public PlatformLayer {
public:
    SDL2Layer();
    void render(std::vector<Buffer*> buffers);
    Viewport getViewport();
    void getFontDimensions(int *width, int *height);
    std::queue<SDL_Event> getEvents();
    void handleResize();
    // TODO: Check for slower machines and drop fps?
    void startLoop();
    void finishLoop();

private:
    SDL_Window *window;
    SDL_Surface *windowSurface;
    TTF_Font *font;

    const uint32_t FPS = 60;
    const uint32_t TICKS_PER_FRAME = 1000 / FPS;
    Timer capTimer;
};

#endif //PYMACS_SDL2LAYER_H
